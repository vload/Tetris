#pragma once

#include "TetrisState.h"

class WindowContext {
   private:
    GLFWwindow* window;
    TetrisState& state;

   public:
    WindowContext(TetrisState& state);
    ~WindowContext();

    void loop();
    bool should_close();

    WindowContext(const WindowContext& other) = delete;
    WindowContext(WindowContext&& other) = delete;
    WindowContext& operator=(const WindowContext& other) = delete;
    WindowContext& operator=(WindowContext&& other) noexcept = delete;
};
