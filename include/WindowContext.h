#pragma once

#include "util.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    throw std::runtime_error(description);
}

class WindowContext {
   private:
    GLFWwindow* window;

   public:
    WindowContext() {
        glfwSetErrorCallback(glfw_error_callback);
        
        // Initialize GLFW
        if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");

        // Tell GLFW what version of OpenGL we are using
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // Create a GLFW window object
        window = glfwCreateWindow(1100, 1100, "Particles", NULL, NULL);
        if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
        glfwMakeContextCurrent(window);

        // Initialize GLAD before calling any OpenGL function
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            glfwTerminate();
            throw std::runtime_error("Failed to initialize GLAD");
        }

        // Tell OpenGL the size of the rendering window
        glViewport(0, 0, 1100, 1100);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // Setup imgui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |=
            ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io.ConfigFlags |=
            ImGuiConfigFlags_DockingEnable;  // IF using Docking Branch
        // io.ConfigDockingWithShift = true;    // IF using Docking Branch

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(
            window,
            true);  // Second param install_callback=true will install
                    // GLFW callbacks and chain to existing ones.
        ImGui_ImplOpenGL3_Init();

        srand(static_cast<unsigned int>(time(NULL)));
    }

    void set_framebuffer_size_callback(GLFWframebuffersizefun callback) {
        glfwSetFramebufferSizeCallback(window, callback);
    }

    ~WindowContext() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    GLFWwindow* get() { return window; }

    void swap_buffers() { glfwSwapBuffers(window); }

    void poll_events() { glfwPollEvents(); }

    bool should_close() { return glfwWindowShouldClose(window); }

    void set_should_close(bool value) {
        glfwSetWindowShouldClose(window, value);
    }

    int get_key(int key) { return glfwGetKey(window, key); }

    int get_width() {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        return width;
    }

    int get_height() {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        return height;
    }

    void get_cursor_pos(double* xpos, double* ypos) {
        glfwGetCursorPos(window, xpos, ypos);
    }

    double get_time() { return glfwGetTime(); }
};
