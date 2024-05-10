#pragma once

#include "TetrisState.h"

class TetrisLogic {
   private:
    // active_tetromino is the last 4 elements of squares
    std::vector<square>& squares;
    bool is_tetromino_active = false;
    TetrisState& state;
    double time_of_last_move_down = 0;
    void new_game();
    bool can_move_to(const square sq[4], glm::vec2 delta = glm::vec2(0, 0));

   public:
    TetrisLogic(TetrisState& state);

    void loop();
};