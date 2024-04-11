#pragma once

#include "Input.h"

// TETRIS_KEYS = {UP, DOWN, LEFT, RIGHT} enum
constexpr enum TetrisKeys {
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

class TetrisInput : public Input {
   private:
    bool keys[TetrisKeys::_COUNT] = {false, false, false, false};

   public:
    TetrisInput(WindowContext& window) : Input(window) {}

    void process() {
        for (int i = 0; i < TetrisKeys::_COUNT; i++) {
            keys[i] |= is_key_pressed(TetrisKeyNamesGLFW[i]);
        }

        // call parent process
        Input::process();
    }

    bool const * const get_state() { return keys; }

    void reset_state() {
        for (int i = 0; i < TetrisKeys::_COUNT; i++) {
            keys[i] = false;
        }
    }
};