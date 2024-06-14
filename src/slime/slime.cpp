#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <imgui.h>

#include <array>
#include <chrono>
#include <cmath>
#include <exception>
#include <glm/ext/matrix_float2x2.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <numbers>
#include <random>
#include <thread>
#include <vector>

#include "../VlEngine/Bind.h"
#include "../VlEngine/BufferObject.h"
#include "../VlEngine/Program.h"
#include "../VlEngine/VertexArrayObject.h"
#include "../VlEngine/WindowContext.h"

constexpr bool enable_vsync = true;
constexpr float pi = std::numbers::pi_v<float>; // NOLINT(readability-identifier-length)
constexpr float min_frame_time = 1 / 144.F;
constexpr int agent_count = 100000;
constexpr int simulation_speed = 1;
constexpr int screen_size = 1100;
constexpr float blur_program_xy_dimensions = 32.F;

struct agent_t {
    glm::vec2 position;
    glm::vec2 direction;
    glm::vec3 color;
    float padding;
};

class FrameBuffer {
   private:
    unsigned int fbo{};
    unsigned int texture{};
    int texture_unit;

   public:
    explicit FrameBuffer(int texture_unit = 0) : texture_unit(texture_unit) {
        glGenFramebuffers(1, &fbo);
        glGenTextures(1, &texture);
        auto framebuffer_bind = Bind(*this);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_size, screen_size, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, texture, 0);
    }
    ~FrameBuffer() {
        glDeleteTextures(1, &texture);
        glDeleteFramebuffers(1, &fbo);
    }
    void bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    void unbind() const {
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glActiveTexture(GL_TEXTURE0);
    }

    void bind(GLenum target) const {
        glBindFramebuffer(target, fbo);
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    void unbind(GLenum target) const {
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(target, 0);
        glActiveTexture(GL_TEXTURE0);
    }

    [[nodiscard]] auto get_texture() const -> unsigned int { return texture; }

    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer(FrameBuffer&&) = delete;
    auto operator=(const FrameBuffer&) -> FrameBuffer& = delete;
    auto operator=(FrameBuffer&&) -> FrameBuffer& = delete;
};

void slime() {
    WindowContext window;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
    ImGui_ImplOpenGL3_Init();

    std::mt19937 random_generator = std::mt19937(static_cast<unsigned int>(
        std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_real_distribution<float> random_position =
        std::uniform_real_distribution<float>(-1.F, 1.F);
    std::uniform_int_distribution<int> random_type =
        std::uniform_int_distribution<int>(0, 2);

    Program render_program("assets/shaders/slime.vert",
                           "assets/shaders/slime.frag");
    Program blur_program("assets/shaders/blur.comp");
    Program think_program("assets/shaders/think.comp");

    FrameBuffer render_frame(0);
    FrameBuffer blur_frame(1);

    // setup agent buffer and vertex array object
    VertexArrayObject agent_vao;
    BufferObject<agent_t> agent_buffer;
    agent_vao.add_attribute(agent_buffer, 0, &agent_t::position);
    agent_vao.add_attribute(agent_buffer, 1, &agent_t::color);

    // generate and copy agent data to buffer
    {
        std::vector<agent_t> agents;
        agents.reserve(agent_count);
        for (int i = 0; i < agent_count; i++) {
            switch (random_type(random_generator)) {
                case 0:
                    agents.push_back({
                        0.F * glm::normalize(
                                  glm::vec2(random_position(random_generator),
                                            random_position(random_generator))),
                        glm::normalize(
                            glm::vec2(random_position(random_generator),
                                      random_position(random_generator))),
                        glm::vec3(1.F, 0.F, 0.F),
                    });
                    break;
                case 1:
                    agents.push_back({
                        0.F * glm::normalize(
                                  glm::vec2(random_position(random_generator),
                                            random_position(random_generator))),
                        glm::normalize(
                            glm::vec2(random_position(random_generator),
                                      random_position(random_generator))),
                        glm::vec3(0.F, 1.F, 0.F),
                    });
                    break;
                case 2:
                    agents.push_back({
                        0.F * glm::normalize(
                                  glm::vec2(random_position(random_generator),
                                            random_position(random_generator))),
                        glm::normalize(
                            glm::vec2(random_position(random_generator),
                                      random_position(random_generator))),
                        glm::vec3(0.F, 0.F, 1.F),
                    });
                    break;
                default:
                    break;
            }
        }
        agent_buffer.copy_data(agents, GL_DYNAMIC_COPY);
    }

    double last_time = WindowContext::get_time();

    // clang-format off
    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    // clang-format on
    int kernel_radius = 1;
    float decay_factor = 0.9F;
    float step_size = 1.F;
    float point_size = std::sqrt(5.F);
    int sensor_radius = 0;
    float sensor_distance = 9;
    float epsilon = 0.F;
    float sensor_angle = pi / 2.F;
    float move_angle = pi / 8.F;
    // clang-format off
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    // clang-format on
    int simulation_count = 0;
    while (!window.should_close()) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        window.loop();

        ImGui::Begin("Slime");
        // clang-format off
        // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        // clang-format on
        ImGui::SliderInt("Kernel Radius", &kernel_radius, 0, 10);
        ImGui::SliderFloat("Decay Factor", &decay_factor, 0.F, 1.F);
        ImGui::SliderFloat("Sensor Angle", &sensor_angle, 0.F, pi);
        ImGui::SliderFloat("Move Angle", &move_angle, 0.F, pi);
        ImGui::SliderFloat("Sensor Distance", &sensor_distance, 0.F, 30.F);
        ImGui::SliderInt("Sensor Radius", &sensor_radius, 0, 10);
        ImGui::SliderFloat("Step Size", &step_size, 0.F, 10.F);
        ImGui::SliderFloat("Point Size", &point_size, 0.F, 10.F);
        ImGui::SliderFloat("Epsilon", &epsilon, 0.F, 0.1F);
        // clang-format off
        // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        // clang-format on
        ImGui::End();
        glPointSize(point_size);

        /// #1 pass: blur old frame
        {
            auto program_bind = Bind(blur_program);
            auto render_framebuffer_bind = Bind(render_frame);
            auto blur_framebuffer_bind = Bind(blur_frame);

            blur_program.set_uniform("kernel_radius", kernel_radius);
            blur_program.set_uniform("decay_factor", decay_factor);

            glBindImageTexture(0, render_frame.get_texture(), 0, GL_FALSE, 0,
                               GL_READ_ONLY, GL_RGBA8);
            glBindImageTexture(1, blur_frame.get_texture(), 0, GL_FALSE, 0,
                               GL_WRITE_ONLY, GL_RGBA8);

            glDispatchCompute(static_cast<GLint>(std::ceil(
                                  screen_size / blur_program_xy_dimensions)),
                              static_cast<GLint>(std::ceil(
                                  screen_size / blur_program_xy_dimensions)),
                              1);
        }

        /// #2 pass: copy blurred frame to render buffer
        {
            auto blur_framebuffer_bind = Bind(blur_frame, GL_READ_FRAMEBUFFER);
            auto render_framebuffer_bind =
                Bind(render_frame, GL_DRAW_FRAMEBUFFER);

            glBlitFramebuffer(0, 0, screen_size, screen_size, 0, 0, screen_size,
                              screen_size, GL_COLOR_BUFFER_BIT, GL_LINEAR);
        }

        /// #3 pass: update agents
        {
            std::array<glm::mat2, 3> sensor_rotation_matrices = {
                glm::mat2(std::cos(sensor_angle), -std::sin(sensor_angle),
                          std::sin(sensor_angle), std::cos(sensor_angle)),
                glm::mat2(std::cos(0), -std::sin(0), std::sin(0), std::cos(0)),
                glm::mat2(std::cos(-sensor_angle), -std::sin(-sensor_angle),
                          std::sin(-sensor_angle), std::cos(-sensor_angle)),
            };
            std::array<glm::mat2, 3> move_rotation_matrices = {
                glm::mat2(std::cos(move_angle), -std::sin(move_angle),
                          std::sin(move_angle), std::cos(move_angle)),
                glm::mat2(std::cos(0), -std::sin(0), std::sin(0), std::cos(0)),
                glm::mat2(std::cos(-move_angle), -std::sin(-move_angle),
                          std::sin(-move_angle), std::cos(-move_angle)),
            };

            auto program_bind = Bind(think_program);
            auto agent_buffer_bind =
                Bind(agent_buffer, GL_SHADER_STORAGE_BUFFER, 0);
            auto blur_framebuffer_bind = Bind(blur_frame);
            glBindImageTexture(1, blur_frame.get_texture(), 0, GL_FALSE, 0,
                               GL_READ_ONLY, GL_RGBA8);

            think_program.set_uniform("sensor_rotation_matrices",
                                      sensor_rotation_matrices);
            think_program.set_uniform("move_rotation_matrices",
                                      move_rotation_matrices);
            think_program.set_uniform("sensor_distance", sensor_distance);
            think_program.set_uniform("sensor_radius", sensor_radius);
            think_program.set_uniform("step_size", step_size);
            think_program.set_uniform("epsilon", epsilon);
            think_program.set_uniform("seed",
                                      random_position(random_generator));

            glDispatchCompute(static_cast<GLint>(std::ceil(agent_count)), 1, 1);
        }

        /// #4 pass: render agents
        {
            auto program_bind = Bind(render_program);
            auto agent_vao_bind = Bind(agent_vao);
            auto agent_buffer_bind = Bind(agent_buffer, GL_ARRAY_BUFFER);
            auto framebuffer_bind = Bind(render_frame);

            glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(agent_count));
        }

        /// #5 pass: draw to screen
        {
            auto framebuffer_bind = Bind(blur_frame, GL_READ_FRAMEBUFFER);
            glBlitFramebuffer(0, 0, screen_size, screen_size, 0, 0, screen_size,
                              screen_size, GL_COLOR_BUFFER_BIT, GL_LINEAR);
        }

        // render imgui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (simulation_count++ % simulation_speed == 0) {
            while (WindowContext::get_time() - last_time < min_frame_time) {
                std::this_thread::sleep_for(std::chrono::microseconds(1));
            }
            last_time = WindowContext::get_time();
        }
    }
}

auto main() -> int { // NOLINT(bugprone-exception-escape)
    try {
        slime();
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return -1;
    } catch (...) {
        std::cerr << "An unknown error occurred\n";
        return -1;
    }
    return 0;
}