#pragma once

// glad and glfw
#include <glad/glad.h>
// This order is important
#include <GLFW/glfw3.h>

// glm
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// std
#include <array>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

class WindowContext {
   public:
    typedef std::function<void(GLFWwindow*, int, int)> FramebufferSizeCallback;

   private:
    GLFWwindow* window;

    std::pair<int, int> get_window_size();
    void trigger_framebuffer_size_callbacks(int width, int height);
    void trigger_framebuffer_size_callbacks();

   public:
    WindowContext();
    ~WindowContext();

    void loop();

    // getters / setters
    bool should_close();
    GLFWwindow* get() { return window; }
    double get_time();
    void add_framebuffer_size_callback(FramebufferSizeCallback callback);

    WindowContext(const WindowContext& other) = delete;
    WindowContext(WindowContext&& other) = delete;
    WindowContext& operator=(const WindowContext& other) = delete;
    WindowContext& operator=(WindowContext&& other) noexcept = delete;
};
