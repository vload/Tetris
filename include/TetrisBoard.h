#pragma once

#include "TetrisInput.h"
#include "WindowContext.h"

class TetrisBoard {
   public:
    struct block {
        glm::vec2 position;
        int type;
    };

    struct wall_vertex {
        glm::vec2 position;
        float color;
    };

   private:
    WindowContext& window;
    TetrisInput& input;

    class GamePiece {};

    std::vector<block> blocks = {};
    bool previous_clear_was_tetris = false;
    bool should_start_new_game = true;
    bool game_over = false;
    int level = 1;
    int lines_cleared_since_level_start = 0;
    int score = 0;
    double last_move_down = 0;
    double fall_delay = 1;
    bool is_piece_locked = false;

    void start_new_game();
    void start_new_level_if_needed();

    bool does_collide_with_inactive_blocks(const block sq[4]);
    bool does_movement_collide_with_inactive_blocks(const block sq[4],
                                                    glm::vec2 delta);
    bool move_current_piece_if_possible(glm::vec2 delta);

    bool is_upcoming_piece_playable();
    void place_upcoming_piece();
    void generate_new_upcoming_piece();
    bool place_upcoming_piece_if_possible(double current_time);

    void handle_horizontal_movement();
    void handle_piece_rotation();
    void handle_piece_falling(double current_time);
    void handle_completed_rows();

    auto get_current_piece();
    auto get_upcoming_piece();
    auto get_inactive_blocks();

    void adjust_score(int rows_cleared);

   public:
    TetrisBoard(WindowContext& window, TetrisInput& input)
        : window(window), input(input) {}

    void loop();

    // getters / setters
    std::vector<block>& get_blocks() { return blocks; }
    std::vector<wall_vertex>& get_wall_vertices();
    bool is_game_over() { return game_over; }
    int get_level() { return level; }
    int get_score() { return score; }
    void initiate_new_game() { should_start_new_game = true; }

    TetrisBoard(TetrisBoard const&) = delete;
    void operator=(TetrisBoard const&) = delete;
    TetrisBoard(TetrisBoard&&) = delete;
    void operator=(TetrisBoard&&) = delete;
};