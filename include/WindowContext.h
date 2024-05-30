#ifndef WINDOWCONTEXT_H_
#define WINDOWCONTEXT_H_

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
    using FramebufferSizeCallback = std::function<void(GLFWwindow*, int, int)>;

   private:
    GLFWwindow* window;

    auto get_window_size() -> std::pair<int, int>;
    void trigger_framebuffer_size_callbacks(int width, int height);
    void trigger_framebuffer_size_callbacks();

   public:
    WindowContext();
    ~WindowContext();

    void loop();

    // getters / setters
    auto is_key_pressed(int key) -> bool;
    auto should_close() -> bool;
    auto get() -> GLFWwindow* { return window; }
    static auto get_time() -> double;
    void add_framebuffer_size_callback(FramebufferSizeCallback& callback);

    WindowContext(const WindowContext& other) = delete;
    WindowContext(WindowContext&& other) = delete;
    auto operator=(const WindowContext& other) -> WindowContext& = delete;
    auto operator=(WindowContext&& other) noexcept -> WindowContext& = delete;
};

#endif  // WINDOWCONTEXT_H_
