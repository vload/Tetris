#include "TetrisLogic.h"

#include <ranges>

TetrisLogic::TetrisLogic(TetrisState& state)
    : state(state), blocks(state.blocks) {}

auto TetrisLogic::get_current_piece() {
    return blocks | std::views::drop(blocks.size() - 8) | std::views::take(4);
}

auto TetrisLogic::get_upcoming_piece() {
    return blocks | std::views::drop(blocks.size() - 4);
}

auto TetrisLogic::get_inactive_blocks() {
    if (is_piece_locked)
        return blocks | std::views::take(blocks.size() - 4);
    else
        return blocks | std::views::take(blocks.size() - 8);
}

void TetrisLogic::start_new_game() {
    srand(static_cast<unsigned int>(time(NULL)));

    blocks.clear();
    create_boundary_walls();

    is_piece_locked = true;
    state.is_game_over = false;
    state.should_start_new_game = false;
    state.score = 0;
    state.level = 1;
    state.clears = 0;

    time_of_last_move_down = 0;
    generate_new_upcoming_piece();
    state.is_scene_dirty = true;
}

void TetrisLogic::create_boundary_walls() {
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

void TetrisLogic::generate_new_upcoming_piece() {
    int tetromino_index = random_int(0, 6);

    for (int i = 0; i < 4; i++)
        blocks.push_back(tetrominos[tetromino_index][i]);

    for (block& s : get_upcoming_piece())
        s.position += UPCOMING_PIECE_PREVIEW_POSITION;
}

bool TetrisLogic::is_upcoming_piece_playable() {
    return !does_movement_collide_with_inactive_blocks(
        get_upcoming_piece().data(),
        PIECE_SPAWN_POSTION - UPCOMING_PIECE_PREVIEW_POSITION);
}

bool TetrisLogic::does_collide_with_inactive_blocks(const block sq[4]) {
    for (const block& s : get_inactive_blocks()) {
        for (int i = 0; i < 4; i++) {
            if (sq[i].position == s.position) {
                return true;
            }
        }
    }
    return false;
}

bool TetrisLogic::does_movement_collide_with_inactive_blocks(const block sq[4],
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

bool TetrisLogic::move_current_piece_if_possible(glm::vec2 delta) {
    if (!does_movement_collide_with_inactive_blocks(get_current_piece().data(),
                                                    delta)) {
        for (block& s : get_current_piece()) {
            s.position += delta;
        }
        return true;
    }
    return false;
}

void TetrisLogic::place_upcoming_piece() {
    for (block& s : get_upcoming_piece())
        s.position =
            s.position - UPCOMING_PIECE_PREVIEW_POSITION + PIECE_SPAWN_POSTION;
}

bool TetrisLogic::place_upcoming_piece_if_possible() {
    if (!is_upcoming_piece_playable()) {
        state.is_game_over = true;
        return false;
    }

    place_upcoming_piece();

    generate_new_upcoming_piece();

    is_piece_locked = false;
    time_of_last_move_down = state.current_time;
    state.is_scene_dirty = true;

    return true;
}

void TetrisLogic::handle_horizontal_movement() {
    int move = 0;
    if (state.keys[TetrisDirections::LEFT].action_needed) {
        move += -1;
    }
    if (state.keys[TetrisDirections::RIGHT].action_needed) {
        move += 1;
    }
    if (move != 0) {
        move_current_piece_if_possible(glm::ivec2(move, 0));
        state.is_scene_dirty = true;
    }
}

void TetrisLogic::handle_piece_falling() {
    // check if tetromino can move down (on delay (gravity) or on key press)
    // try to move it and if it can't be moved, then deactivate it
    if (state.current_time - time_of_last_move_down > fall_delay ||
        state.keys[TetrisDirections::DOWN].action_needed) {
        if (!move_current_piece_if_possible(glm::ivec2(0, 1)))
            is_piece_locked = true;

        time_of_last_move_down = state.current_time;
        state.is_scene_dirty = true;
    }
}

void TetrisLogic::handle_piece_rotation() {
    if (state.keys[TetrisDirections::UP].action_needed) {
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
            state.is_scene_dirty = true;
        }
    }
}

void TetrisLogic::loop() {
    state.is_scene_dirty = false;

    if (state.should_start_new_game) {
        start_new_game();
    }

    if (is_piece_locked) {
        if (!place_upcoming_piece_if_possible()) return;
    }

    handle_piece_rotation();

    handle_horizontal_movement();

    handle_piece_falling();

    handle_completed_rows();
}

void TetrisLogic::handle_completed_rows() {
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
        size_t rows_cleared =
            std::erase_if(blocks,
                          [&](block s) {
                              return s.position.x > 1 && s.position.x < 12 &&
                                     s.position.y > 0 && s.position.y < 20 &&
                                     count_per_line[(int)s.position.y] == 10;
                          }) /
            10;

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

        if (rows_cleared > 0) {
            state.is_scene_dirty = true;
        }
    }
}

void TetrisLogic::adjust_score(size_t rows_cleared) {
    static const int points_per_row[6] = {0, 100, 300, 700, 1500, 3100};
    if (rows_cleared == 0) return;

    if (rows_cleared == 4) {
        if (state.previous_clear_was_tetris) {  // back to back tetris
            rows_cleared++;
        }

        state.previous_clear_was_tetris = true;
    } else {
        state.previous_clear_was_tetris = false;
    }

    state.score += points_per_row[rows_cleared] * state.level;
    state.clears += rows_cleared;
    if(state.clears >= 10) {
        state.level++;
        state.clears -= 10;
    }
}