#ifndef WINDOWCONTEXT_H_
#define WINDOWCONTEXT_H_

#include <glad/glad.h>
// This order is important
#include <GLFW/glfw3.h>

// std
#include <array>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "Events/EventBus.h"

class WindowContext {
   public:
    using FramebufferSizeCallback = std::function<void(GLFWwindow*, int, int)>;

    auto get_window_size() -> std::pair<int, int>;
   private:
    GLFWwindow* window;

    void trigger_framebuffer_size_callbacks(int width, int height);
    void trigger_framebuffer_size_callbacks();

   public:
    WindowContext();
    WindowContext(EventBus& event_bus, int width, int height);
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
