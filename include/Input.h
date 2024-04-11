#pragma once

#include "WindowContext.h"

class Input {
   private:
    WindowContext window;

   public:
    Input(WindowContext& window) : window(window) {}

    bool is_key_pressed(int key) {
        return glfwGetKey(window.get(), key) == GLFW_PRESS;
    }

    void process() {
        if (is_key_pressed(GLFW_KEY_ESCAPE)) window.set_should_close(true);
    }
};