#pragma once

#include "util.h"

class TetrisLogic {
   private:
    const glm::vec2 UPCOMING_PIECE_PREVIEW_POSITION{17, 11};
    const glm::vec2 PIECE_SPAWN_POSTION{7, 0};
    const double fall_delay = 0.5;

    TetrisState& state;
    std::vector<block>& blocks;

    bool is_piece_locked = false;
    double time_of_last_move_down = 0;

    void start_new_game();
    void create_boundary_walls();

    bool does_collide_with_inactive_blocks(const block sq[4]);
    bool does_movement_collide_with_inactive_blocks(const block sq[4],
                                                    glm::vec2 delta);
    bool move_current_piece_if_possible(glm::vec2 delta);

    bool is_upcoming_piece_playable();
    void place_upcoming_piece();
    void generate_new_upcoming_piece();
    bool place_upcoming_piece_if_possible();

    void handle_horizontal_movement();
    void handle_piece_rotation();
    void handle_piece_falling();
    void handle_completed_rows();

    auto get_current_piece();
    auto get_upcoming_piece();
    auto get_inactive_blocks();

    void adjust_score(size_t rows_cleared);

   public:
    TetrisLogic(TetrisState& state);

    void loop();
};