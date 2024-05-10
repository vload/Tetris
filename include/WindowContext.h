#pragma once

#include "TetrisState.h"

class WindowContext {
   private:
    GLFWwindow* window;
    TetrisState& state;

    int get_width();
    int get_height();

   public:
    WindowContext(TetrisState& state);
    ~WindowContext();

    void loop();

    GLFWwindow* get();
    bool should_close();
    int get_key(int key);
    double get_time();
    void set_should_close(bool value);

    WindowContext(const WindowContext& other) = delete;
    WindowContext(WindowContext&& other) = delete;
    WindowContext& operator=(const WindowContext& other) = delete;
    WindowContext& operator=(WindowContext&& other) noexcept = delete;
};
