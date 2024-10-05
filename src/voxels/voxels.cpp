#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <imgui.h>

#include <array>
#include <chrono>
#include <cmath>
#include <exception>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float2x2.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <numbers>
#include <random>
#include <thread>
#include <vector>

#include "../VlEngine/Bind.h"
#include "../VlEngine/BufferObject.h"
#include "../VlEngine/Events/EventBus.h"
#include "../VlEngine/Events/KeyboardEvents.h"
#include "../VlEngine/Events/WindowEvents.h"
#include "../VlEngine/FrameBuffer.h"
#include "../VlEngine/Program.h"
#include "../VlEngine/Texture.h"
#include "../VlEngine/VertexArrayObject.h"
#include "../VlEngine/WindowContext.h"

constexpr int terrain_size = 2;
constexpr int sphere_radius = 12;
constexpr float voxel_size = 1.0f;
constexpr float camera_speed = 20.0f;
constexpr float sensitivity = 0.1f;
constexpr float fov = glm::radians(45.0f);
constexpr int ray_program_x_dim = 32;

struct alignas(16) voxel_t {
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 color;
};

struct alignas(16) ray_t {
    alignas(16) glm::vec3 origin;
    alignas(16) glm::vec3 direction;
    alignas(16) glm::ivec2 pixel_coords;
};

class Timer {
    std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
    double& time;

   public:
    Timer(double& time) : time(time) {
        startTimePoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        auto endTimePoint = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = endTimePoint - startTimePoint;
        time = duration.count();
    }
};

glm::vec3 camera_position{0.1f, 0.1f, 5.1f};
glm::vec3 camera_direction{0, 0, 0};
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
double last_time = WindowContext::get_time();
double pitch = 0;
double yaw = -56.0f;
double roll = 0;

double lastX = 400, lastY = 300;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        if (firstMouse)  // initially set to true
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        double xoffset = xpos - lastX;
        double yoffset =
            lastY -
            ypos;  // reversed since y-coordinates range from bottom to top
        lastX = xpos;
        lastY = ypos;

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        camera_direction.x = static_cast<float>(cos(glm::radians(yaw)) *
                                                cos(glm::radians(pitch)));
        camera_direction.y = static_cast<float>(sin(glm::radians(pitch)));
        camera_direction.z = static_cast<float>(sin(glm::radians(yaw)) *
                                                cos(glm::radians(pitch)));
    } else {
        firstMouse = true;
    }
}

void voxels() {
    EventBus event_bus;

    WindowContext window(event_bus, 1280, 720);

    // glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
    ImGui_ImplOpenGL3_Init();

    std::mt19937 random_generator = std::mt19937(static_cast<unsigned int>(
        std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> random_int =
        std::uniform_int_distribution<int>(0, 2);

    Program voxel_program("assets/shaders/voxels.vert",
                          "assets/shaders/voxels.geom",
                          "assets/shaders/voxels.frag");

    Program ray_gen_program("assets/shaders/ray_gen.comp");
    Program ray_marching_program("assets/shaders/ray_marching.comp");

    Texture render_texture(GL_TEXTURE_2D, 0);
    FrameBuffer render_frame(event_bus, render_texture);

    auto [width, height] = window.get_window_size();
    event_bus.subscribe<WindowResizeEvent>(
        [&width, &height](WindowResizeEvent event) {
            width = event.width;
            height = event.height;
        });

    BufferObject<voxel_t> voxel_buffer;

    BufferObject<ray_t> ray_buffer;
    event_bus.subscribe<WindowResizeEvent>(
        [&ray_buffer](const WindowResizeEvent& event) {
            ray_buffer.allocate(event.width * event.height, GL_DYNAMIC_COPY);
        });

    // generate and copy agent data to buffer
    std::vector<voxel_t> voxels;
    int voxel_count = 0;
    {
        for (int i = -sphere_radius; i <= sphere_radius; i++) {
            for (int j = -sphere_radius; j <= sphere_radius; j++) {
                for (int k = -sphere_radius; k <= sphere_radius; k++) {
                    if (i * i + j * j + k * k < sphere_radius * sphere_radius) {
                        glm::vec3 color{
                            (float)(i + sphere_radius) / sphere_radius / 2,
                            (float)(j + sphere_radius) / sphere_radius / 2,
                            (float)(k + sphere_radius) / sphere_radius / 2,
                        };

                        voxels.push_back(voxel_t{
                            {i * voxel_size, k * voxel_size, j * voxel_size},
                            color});

                        voxel_count++;
                    }
                }
            }
        }
        voxel_buffer.copy_data(voxels, GL_DYNAMIC_COPY);
        std::cout << voxel_count << std::endl;
    }

    glfwSetCursorPosCallback(window.get(), mouse_callback);

    /// setup input event listening
    std::array<bool, GLFW_KEY_LAST + 1> pressed_keys{false};
    event_bus.subscribe<KeyPressEvent>([&](const KeyPressEvent& event) {
        pressed_keys[event.key_code] = true;
    });

    event_bus.subscribe<KeyReleaseEvent>([&](const KeyReleaseEvent& event) {
        pressed_keys[event.key_code] = false;
    });

    double pass_1_time = 0;
    double pass_2_time = 0;
    double pass_3_time = 0;
    double total_frame_time = 0;

    while (!window.should_close()) {
        Timer total_timer(total_frame_time);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        window.loop();
        event_bus.dispatch();

        ImGui::Begin("Voxels");
        // clang-format off
        // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        // clang-format on
        ImGui::Text("#1 pass: %lf", pass_1_time);
        ImGui::Text("#2 pass: %lf", pass_2_time);
        ImGui::Text("#3 pass: %lf", pass_3_time);
        ImGui::Text("total frame time: %lf", total_frame_time);
        if (total_frame_time > 1 / 60.0) {
            ImGui::Text("TOO SLOW!");
        }
        // ImGui::Text("x: %f", camera_position.x);
        // ImGui::Text("y: %f", camera_position.y);
        // ImGui::Text("z: %f", camera_position.z);
        // clang-format off
        // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        // clang-format on
        ImGui::End();

        double delta_time = WindowContext::get_time() - last_time;
        last_time = delta_time + last_time;

        // process input for camera movement
        {
            if (pressed_keys[GLFW_KEY_W]) {
                camera_position +=
                    camera_speed * camera_direction * (float)delta_time;
            }
            if (pressed_keys[GLFW_KEY_S]) {
                camera_position -=
                    camera_speed * camera_direction * (float)delta_time;
            }
            if (pressed_keys[GLFW_KEY_A]) {
                camera_position -=
                    glm::normalize(glm::cross(camera_direction, up)) *
                    camera_speed * (float)delta_time;
            }
            if (pressed_keys[GLFW_KEY_D]) {
                camera_position +=
                    glm::normalize(glm::cross(camera_direction, up)) *
                    camera_speed * (float)delta_time;
            }
            if (pressed_keys[GLFW_KEY_SPACE]) {
                camera_position += up * camera_speed * (float)delta_time;
            }
            if (pressed_keys[GLFW_KEY_LEFT_SHIFT]) {
                camera_position -= up * camera_speed * (float)delta_time;
            }
        }

        /// #1 pass: ray generation
        {
            Timer timer(pass_1_time);

            auto program_bind = Bind(ray_gen_program);
            auto buffer_bind = Bind(ray_buffer, GL_SHADER_STORAGE_BUFFER, 1);

            // uniforms
            glm::vec3 camera_right =
                glm::normalize(glm::cross(up, camera_direction));
            glm::vec3 camera_up = glm::cross(camera_direction, camera_right);
            float tanfov = tan(fov / 2);
            ray_gen_program.set_uniform("width", width);
            ray_gen_program.set_uniform("height", height);
            ray_gen_program.set_uniform("tanfov", tanfov);
            ray_gen_program.set_uniform("camera_direction", camera_direction);
            ray_gen_program.set_uniform("camera_position", camera_position);
            ray_gen_program.set_uniform("camera_right", camera_right);
            ray_gen_program.set_uniform("camera_up", camera_up);

            glDispatchCompute(
                static_cast<GLint>(std::ceil((float)width / ray_program_x_dim)),
                static_cast<GLint>(
                    std::ceil((float)height / ray_program_x_dim)),
                1);
            glMemoryBarrier(GL_ALL_BARRIER_BITS);
            glFinish();
        }

        /// #2 pass: ray marching?
        {
            Timer timer(pass_2_time);

            auto program_bind = Bind(ray_marching_program);
            auto buffer_bind = Bind(ray_buffer, GL_SHADER_STORAGE_BUFFER, 1);
            auto voxel_buffer_bind =
                Bind(voxel_buffer, GL_SHADER_STORAGE_BUFFER, 2);
            auto render_texture_bind = Bind(render_texture, GL_WRITE_ONLY);

            ray_marching_program.set_uniform("voxel_count", voxel_count);
            ray_marching_program.set_uniform("voxel_size", voxel_size);

            glDispatchCompute(static_cast<GLint>(std::ceil(
                                  (float)width * height / ray_program_x_dim)),
                              1, 1);
            glMemoryBarrier(GL_ALL_BARRIER_BITS);
            glFinish();
        }

        /// #3 pass: blit framebuffer to screen
        {
            Timer timer(pass_3_time);

            auto framebuffer_bind = Bind(render_frame, GL_READ_FRAMEBUFFER);

            glBlitFramebuffer(0, 0, width, height, 0, 0, width, height,
                              GL_COLOR_BUFFER_BIT, GL_LINEAR);
            glFinish();
        }

        // render imgui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

auto main() -> int {  // NOLINT(bugprone-exception-escape)
    try {
        voxels();
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return -1;
    } catch (...) {
        std::cerr << "An unknown error occurred\n";
        return -1;
    }
    return 0;
}