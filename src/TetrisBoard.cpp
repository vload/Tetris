#include "TetrisBoard.h"

#include <algorithm>
#include <ranges>

void set_seed() { srand(static_cast<unsigned int>(time(0))); }
int random_int(int min, int max) { return rand() % (max - min + 1) + min; };

constexpr glm::vec2 UPCOMING_PIECE_PREVIEW_POSITION{17, 11};
constexpr glm::vec2 PIECE_SPAWN_POSTION{7, 0};

constexpr TetrisBoard::block tetrominos[][4] = {
    {
        // O
        {glm::ivec2(0, 0), 1},
        {glm::ivec2(-1, 0), 1},
        {glm::ivec2(-1, 1), 1},
        {glm::ivec2(0, 1), 1},
    },
    {
        // I
        {glm::ivec2(0, 0), 2},
        {glm::ivec2(-1, 0), 2},
        {glm::ivec2(1, 0), 2},
        {glm::ivec2(-2, 0), 2},
    },
    {
        // T
        {glm::ivec2(0, 0), 3},
        {glm::ivec2(-1, 0), 3},
        {glm::ivec2(1, 0), 3},
        {glm::ivec2(0, 1), 3},
    },
    {
        // L
        {glm::ivec2(0, 0), 4},
        {glm::ivec2(-1, 0), 4},
        {glm::ivec2(1, 0), 4},
        {glm::ivec2(-1, 1), 4},
    },
    {
        // J
        {glm::ivec2(0, 0), 5},
        {glm::ivec2(-1, 0), 5},
        {glm::ivec2(1, 0), 5},
        {glm::ivec2(1, 1), 5},
    },
    {
        // Z
        {glm::ivec2(0, 0), 6},
        {glm::ivec2(-1, 0), 6},
        {glm::ivec2(0, 1), 6},
        {glm::ivec2(1, 1), 6},
    },
    {
        // S
        {glm::ivec2(0, 0), 7},
        {glm::ivec2(1, 0), 7},
        {glm::ivec2(0, 1), 7},
        {glm::ivec2(-1, 1), 7},
    },
};

auto TetrisBoard::get_current_piece() {
    return blocks | std::views::drop(blocks.size() - 8) | std::views::take(4);
}

auto TetrisBoard::get_upcoming_piece() {
    return blocks | std::views::drop(blocks.size() - 4);
}

auto TetrisBoard::get_inactive_blocks() {
    if (is_piece_locked)
        return blocks | std::views::take(blocks.size() - 4);
    else
        return blocks | std::views::take(blocks.size() - 8);
}

void TetrisBoard::start_new_game() {
    srand(static_cast<unsigned int>(time(NULL)));

    blocks.clear();
    create_boundary_walls();

    is_piece_locked = true;
    game_over = false;
    should_start_new_game = false;
    score = 0;
    level = 1;
    lines_cleared_since_level_start = 0;

    last_move_down = 0;
    generate_new_upcoming_piece();
}

void TetrisBoard::create_boundary_walls() {
    // side walls
    for (int i = -5; i < 20; i++) {
        blocks.push_back({glm::ivec2(1, i), 0});
        blocks.push_back({glm::ivec2(12, i), 0});
    }
    // bottom wall
    for (int i = 1; i < 13; i++) {
        blocks.push_back({glm::ivec2(i, 20), 0});
    }
}

void TetrisBoard::generate_new_upcoming_piece() {
    int tetromino_index = random_int(0, 6);

    for (int i = 0; i < 4; i++)
        blocks.push_back(tetrominos[tetromino_index][i]);

    for (block& s : get_upcoming_piece())
        s.position += UPCOMING_PIECE_PREVIEW_POSITION;
}

bool TetrisBoard::is_upcoming_piece_playable() {
    return !does_movement_collide_with_inactive_blocks(
        get_upcoming_piece().data(),
        PIECE_SPAWN_POSTION - UPCOMING_PIECE_PREVIEW_POSITION);
}

bool TetrisBoard::does_collide_with_inactive_blocks(const block sq[4]) {
    for (const block& s : get_inactive_blocks()) {
        for (int i = 0; i < 4; i++) {
            if (sq[i].position == s.position) {
                return true;
            }
        }
    }
    return false;
}

bool TetrisBoard::does_movement_collide_with_inactive_blocks(const block sq[4],
                                                             glm::vec2 delta) {
    for (const block& s : get_inactive_blocks()) {
        for (int i = 0; i < 4; i++) {
            if (sq[i].position + delta == s.position) {
                return true;
            }
        }
    }
    return false;
}

bool TetrisBoard::move_current_piece_if_possible(glm::vec2 delta) {
    if (!does_movement_collide_with_inactive_blocks(get_current_piece().data(),
                                                    delta)) {
        for (block& s : get_current_piece()) {
            s.position += delta;
        }
        return true;
    }
    return false;
}

void TetrisBoard::place_upcoming_piece() {
    for (block& s : get_upcoming_piece())
        s.position =
            s.position - UPCOMING_PIECE_PREVIEW_POSITION + PIECE_SPAWN_POSTION;
}

bool TetrisBoard::place_upcoming_piece_if_possible(double current_time) {
    if (!is_upcoming_piece_playable()) {
        game_over = true;
        return false;
    }

    place_upcoming_piece();

    generate_new_upcoming_piece();

    is_piece_locked = false;
    last_move_down = current_time;

    return true;
}

void TetrisBoard::handle_horizontal_movement() {
    int move = 0;
    if (input.get_keys()[TetrisInput::TetrisDirections::LEFT].action_needed) {
        move += -1;
    }
    if (input.get_keys()[TetrisInput::TetrisDirections::RIGHT].action_needed) {
        move += 1;
    }
    if (move != 0) {
        move_current_piece_if_possible(glm::ivec2(move, 0));
    }
}

void TetrisBoard::handle_piece_falling(double current_time) {
    // check if tetromino can move down (on delay (gravity) or on key press)
    // try to move it and if it can't be moved, then deactivate it
    if (current_time - last_move_down > fall_delay ||
        input.get_keys()[TetrisInput::TetrisDirections::DOWN].action_needed) {
        if (!move_current_piece_if_possible(glm::ivec2(0, 1)))
            is_piece_locked = true;

        last_move_down = current_time;
    }
}

void TetrisBoard::handle_piece_rotation() {
    if (input.get_keys()[TetrisInput::TetrisDirections::UP].action_needed) {
        block sq[4];
        std::copy(get_current_piece().begin(), get_current_piece().end(), sq);

        // calculate center of rotation
        glm::vec2 center = sq[0].position;

        // rotate around center
        for (int i = 0; i < 4; i++) {
            glm::vec2 delta = sq[i].position - center;
            sq[i].position = center + glm::vec2(-delta.y, delta.x);
        }

        // check if tetromino can move to new spot and move it
        if (!does_collide_with_inactive_blocks(sq)) {
            std::copy(sq, sq + 4, get_current_piece().begin());
        }
    }
}

void TetrisBoard::loop() {
    double current_time = glfwGetTime();

    if (should_start_new_game) {
        start_new_game();
    }

    if (is_piece_locked) {
        if (!place_upcoming_piece_if_possible(current_time)) return;
    }

    handle_piece_rotation();

    handle_horizontal_movement();

    handle_piece_falling(current_time);

    handle_completed_rows();
}

void TetrisBoard::handle_completed_rows() {
    if (is_piece_locked) {
        // check if any lines are full (only in the play area)
        int count_per_line[20] = {0};
        for (int j = 0; j < blocks.size(); j++) {
            if (blocks[j].position.x > 1 && blocks[j].position.x < 12 &&
                blocks[j].position.y >= 0 && blocks[j].position.y < 20) {
                count_per_line[(int)blocks[j].position.y]++;
            }
        }

        // remove full lines
        int rows_cleared =
            static_cast<int>(std::erase_if(blocks,
                          [&](block s) {
                              return s.position.x > 1 && s.position.x < 12 &&
                                     s.position.y > 0 && s.position.y < 20 &&
                                     count_per_line[(int)s.position.y] == 10;
                          }) /
            10);

        // calculate how many lines to move the lines down by
        int move_line_down_by[20] = {0};
        for (int i = 18; i >= 0; i--) {
            move_line_down_by[i] =
                move_line_down_by[i + 1] + (count_per_line[i + 1] == 10);
        }

        // move lines down
        for (int i = 0; i < blocks.size(); i++) {
            if (blocks[i].position.x > 1 && blocks[i].position.x < 12 &&
                blocks[i].position.y < 20) {
                blocks[i].position.y +=
                    move_line_down_by[(int)blocks[i].position.y];
            }
        }

        adjust_score(rows_cleared);
    }
}

void TetrisBoard::adjust_score(int rows_cleared) {
    static const int points_per_row[6] = {0, 100, 300, 700, 1500, 3100};
    if (rows_cleared == 0) return;

    if (rows_cleared == 4) {
        if (previous_clear_was_tetris) {  // back to back tetris
            rows_cleared++;
        }

        previous_clear_was_tetris = true;
    } else {
        previous_clear_was_tetris = false;
    }

    score += points_per_row[rows_cleared] * level;
    lines_cleared_since_level_start += rows_cleared;
    if (lines_cleared_since_level_start >= 10) {
        level++;
        lines_cleared_since_level_start -= 10;
    }
}