#include "WindowContext.h"

#include "settings.h"

std::vector<WindowContext::FramebufferSizeCallback>
    framebuffer_size_callback_chain;

static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
    throw std::runtime_error(description);
}

WindowContext::WindowContext() {
    glfwSetErrorCallback(glfw_error_callback);

    // Initialize GLFW
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Tell GLFW which version of OpenGL to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window object
    window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
                              DEFAULT_WINDOW_TITLE, 0, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD before calling any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glfwSetFramebufferSizeCallback(
        window, [](GLFWwindow* window, int width, int height) {
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

std::pair<int, int> WindowContext::get_window_size() {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    return std::make_pair(width, height);
}

double WindowContext::get_time() { return glfwGetTime(); }

bool WindowContext::should_close() { return glfwWindowShouldClose(window); }

void WindowContext::add_framebuffer_size_callback(
    FramebufferSizeCallback callback) {
    framebuffer_size_callback_chain.push_back(callback);

    trigger_framebuffer_size_callbacks();
}
