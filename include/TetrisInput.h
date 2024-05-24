#pragma once

#include "TetrisState.h"

class TetrisInput {
   private:
    TetrisState& state;
    std::array<KeyStatus, TetrisDirections::_COUNT>& keys;

    bool is_key_pressed(int key);

   public:
    TetrisInput(TetrisState& state);

    void loop();
};