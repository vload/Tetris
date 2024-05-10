#pragma once

#include "TetrisState.h"

class TetrisInput {
   private:
    TetrisState& state;
    double last_read = 0;
    KeyStatus* keys;

   public:
    TetrisInput(TetrisState& state);

    void loop();

    bool is_key_pressed(int key);
    KeyStatus* get_keys();
    void reset_keys();
};