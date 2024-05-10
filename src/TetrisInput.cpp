#include "TetrisInput.h"

TetrisInput::TetrisInput(TetrisState& state) : state(state), keys(&state.keys[0]) {}

void TetrisInput::loop() {
    double current_time = glfwGetTime();

    for (int i = 0; i < TetrisDirections::_COUNT; i++) {
        keys[i].action_needed = false;

        if (is_key_pressed(TetrisKeyNamesGLFW[i]) && !keys[i].pressed) {
            keys[i].time_pressed = current_time;
            keys[i].action_needed = true;
            keys[i].prev_x = -1;
        }

        keys[i].pressed = is_key_pressed(TetrisKeyNamesGLFW[i]);

        double x = floor((current_time - state.delay1 - keys[i].time_pressed) /
                         state.delay2);

        if (keys[i].pressed && x > keys[i].prev_x) {
            keys[i].action_needed = true;
            keys[i].prev_x = x;
        }
    }

    // if (is_key_pressed(GLFW_KEY_ESCAPE)) state.window.set_should_close(true);
}

bool TetrisInput::is_key_pressed(int key) {
    return glfwGetKey(state.window, key) == GLFW_PRESS;
}

KeyStatus* TetrisInput::get_keys() {
    last_read = glfwGetTime();
    return keys;
}

void TetrisInput::reset_keys() {
    for (int i = 0; i < TetrisDirections::_COUNT; i++) {
        keys[i] = {0};
    }
}