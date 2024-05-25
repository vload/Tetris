#pragma once

#include "util.h"

class TetrisInput {
   private:
    const double delay_after_first_key_press = 0.25;
    const double repeat_key_delay = 0.05;
    TetrisState& state;
    std::array<KeyStatus, TetrisDirections::_COUNT>& keys;

    bool is_key_pressed(int key);

   public:
    TetrisInput(TetrisState& state);

    void loop();
};