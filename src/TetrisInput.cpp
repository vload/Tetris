#include "TetrisInput.h"

TetrisInput::TetrisInput(TetrisState& state) : state(state), keys(state.keys) {}

void TetrisInput::loop() {
    for (int i = 0; i < TetrisDirections::_COUNT; i++) {
        keys[i].action_needed = false;

        if (is_key_pressed(TetrisKeyNamesGLFW[i]) && !keys[i].pressed) {
            keys[i].time_pressed = state.current_time;
            keys[i].action_needed = true;
            keys[i].prev_x = -1;
        }

        keys[i].pressed = is_key_pressed(TetrisKeyNamesGLFW[i]);

        double x = floor((state.current_time - delay_after_first_key_press -
                          keys[i].time_pressed) /
                         repeat_key_delay);

        if (keys[i].pressed && x > keys[i].prev_x) {
            keys[i].action_needed = true;
            keys[i].prev_x = x;
        }
    }
}

bool TetrisInput::is_key_pressed(int key) {
    return glfwGetKey(state.window, key) == GLFW_PRESS;
}
