#include "WindowContext.h"

constexpr const char* DEFAULT_WINDOW_TITLE = "Tetris";
constexpr int DEFAULT_WINDOW_WIDTH = 1100;
constexpr int DEFAULT_WINDOW_HEIGHT = 1100;
constexpr int OPENGL_VERSION_MAJOR = 4;
constexpr int OPENGL_VERSION_MINOR = 6;

std::vector<WindowContext::FramebufferSizeCallback>
    framebuffer_size_callback_chain;  // NOLINT

void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << "\n";
    throw std::runtime_error(description);
}

WindowContext::WindowContext() {
    glfwSetErrorCallback(glfw_error_callback);

    // Initialize GLFW
    if (glfwInit() == 0) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Tell GLFW which version of OpenGL to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window object
    window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
                              DEFAULT_WINDOW_TITLE, nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD before calling any OpenGL function
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {  // NOLINT
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glfwSetFramebufferSizeCallback(
        window, [](GLFWwindow* window, int width, int height) { // NOLINT
            for (auto& callback : framebuffer_size_callback_chain) {
                callback(window, width, height);
            }
        });

    FramebufferSizeCallback default_framebuffer_size_callback =
        [](GLFWwindow*, int width, int height) {
            glViewport(0, 0, width, height);
        };
    add_framebuffer_size_callback(default_framebuffer_size_callback);
}

void WindowContext::loop() {
    // swap the buffers
    glfwSwapBuffers(window);
    // check and call events
    glfwPollEvents();
}

WindowContext::~WindowContext() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void WindowContext::trigger_framebuffer_size_callbacks() {
    auto [width, height] = get_window_size();
    trigger_framebuffer_size_callbacks(width, height);
}

void WindowContext::trigger_framebuffer_size_callbacks(int width, int height) {
    for (auto& callback : framebuffer_size_callback_chain) {
        callback(window, width, height);
    }
}

auto WindowContext::get_window_size() -> std::pair<int, int> {
    int width{};
    int height{};
    glfwGetFramebufferSize(window, &width, &height);
    return std::make_pair(width, height);
}

auto WindowContext::get_time() -> double { return glfwGetTime(); }

auto WindowContext::should_close() -> bool {
    return glfwWindowShouldClose(window) != 0;
}

void WindowContext::add_framebuffer_size_callback(
    FramebufferSizeCallback& callback) {
    framebuffer_size_callback_chain.push_back(callback);

    trigger_framebuffer_size_callbacks();
}

auto WindowContext::is_key_pressed(int key) -> bool {
    return glfwGetKey(window, key) == GLFW_PRESS;
}