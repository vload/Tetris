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
    window = glfwCreateWindow(state.width, state.height, "Tetris", 0, NULL);
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
    glViewport(0, 0, state.width, state.height);
    glfwSetFramebufferSizeCallback(window, default_framebuffer_size_callback);

    state.window = window;
}

void WindowContext::loop() {
    // set the current time
    state.current_time = glfwGetTime();
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

bool WindowContext::should_close() { return glfwWindowShouldClose(window); }
