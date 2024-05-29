#pragma once

#include "WindowContext.h"

class TetrisInput {
   public:
    struct KeyStatus {
        bool pressed;
        double time_pressed;
        double prev_x;
        bool action_needed;
    };

    const enum TetrisDirections {
        UP = 0,
        DOWN = 1,
        LEFT = 2,
        RIGHT = 3,
        _COUNT = 4,
    };

   private:
    WindowContext& window;

    std::array<KeyStatus, TetrisDirections::_COUNT> keys;

    bool is_key_pressed(int key);

   public:
    TetrisInput(WindowContext& window);

    void loop();

    // getters / setters
    std::array<KeyStatus, TetrisDirections::_COUNT>& get_keys() { return keys; }
};