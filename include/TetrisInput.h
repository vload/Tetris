#pragma once

#include "Input.h"

double delay1 = 0.25;
double delay2 = 0.05;

constexpr enum TetrisDirections {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3,
    _COUNT = 4,
};

constexpr int TetrisKeyNamesGLFW[] = {
    GLFW_KEY_UP,
    GLFW_KEY_DOWN,
    GLFW_KEY_LEFT,
    GLFW_KEY_RIGHT,
};

struct KeyStatus {
    bool pressed;
    double time_pressed;
    double prev_x;
    bool action_needed;
};

class TetrisInput : public Input {
   private:
    double last_read = 0;
    KeyStatus keys[TetrisDirections::_COUNT] = {0};

   public:
    TetrisInput(WindowContext& window) : Input(window) {}

    void process() {
        double current_time = glfwGetTime();

        for (int i = 0; i < TetrisDirections::_COUNT; i++) {
            keys[i].action_needed = false;

            if (is_key_pressed(TetrisKeyNamesGLFW[i]) && !keys[i].pressed) {
                keys[i].time_pressed = current_time;
                keys[i].action_needed = true;
                keys[i].prev_x = -1;
            }

            keys[i].pressed = is_key_pressed(TetrisKeyNamesGLFW[i]);

            double x =
                floor((current_time - delay1 - keys[i].time_pressed) / delay2);

            if (keys[i].pressed && x > keys[i].prev_x) {
                keys[i].action_needed = true;
                keys[i].prev_x = x;
            }
        }

        // call parent process
        Input::process();
    }

    KeyStatus* get_state() {
        last_read = glfwGetTime();
        return keys;
    }

    double get_last_read() { return last_read; }

    void reset_state() {
        for (int i = 0; i < TetrisDirections::_COUNT; i++) {
            keys[i] = {0};
        }
    }
};