#include "TetrisInput.h"

#include "settings.h"

constexpr int TetrisKeyNamesGLFW[TetrisInput::TetrisDirections::_COUNT] = {
    GLFW_KEY_UP,
    GLFW_KEY_DOWN,
    GLFW_KEY_LEFT,
    GLFW_KEY_RIGHT,
};

TetrisInput::TetrisInput(WindowContext& window) : window(window) {}

void TetrisInput::loop() {
    double current_time = window.get_time();

    for (int i = 0; i < TetrisDirections::_COUNT; i++) {
        keys[i].action_needed = false;

        if (is_key_pressed(TetrisKeyNamesGLFW[i]) && !keys[i].pressed) {
            keys[i].time_pressed = current_time;
            keys[i].action_needed = true;
            keys[i].prev_x = -1;
        }

        keys[i].pressed = is_key_pressed(TetrisKeyNamesGLFW[i]);

        double x = floor((current_time - DELAY_AFTER_FIRST_KEY_PRESS -
                          keys[i].time_pressed) /
                         REPEAT_KEY_DELAY);

        if (keys[i].pressed && x > keys[i].prev_x) {
            keys[i].action_needed = true;
            keys[i].prev_x = x;
        }
    }
}

bool TetrisInput::is_key_pressed(int key) {
    return glfwGetKey(window.get(), key) == GLFW_PRESS;
}
