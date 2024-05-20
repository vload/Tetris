#include "WindowContext.h"

void default_framebuffer_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
    throw std::runtime_error(description);
}

WindowContext::WindowContext(TetrisState& state) : state(state) {
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
    window = glfwCreateWindow(1100, 1100, "Tetris", 0, NULL);
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

    // Tell OpenGL the size of the rendering window
    glViewport(0, 0, get_width(), get_height());
    glfwSetFramebufferSizeCallback(window, default_framebuffer_size_callback);

    state.window = window;
}

void WindowContext::loop() {
    // swap the buffers
    glfwSwapBuffers(window);
    // check and call events
    glfwPollEvents();
    // update the window size
    glfwGetWindowSize(window, &state.width, &state.height);
}

WindowContext::~WindowContext() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

GLFWwindow* WindowContext::get() { return window; }

bool WindowContext::should_close() { return glfwWindowShouldClose(window); }

void WindowContext::set_should_close(bool value) {
    glfwSetWindowShouldClose(window, value);
}

int WindowContext::get_key(int key) { return glfwGetKey(window, key); }

int WindowContext::get_width() {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return width;
}

int WindowContext::get_height() {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return height;
}

double WindowContext::get_time() { return glfwGetTime(); }
