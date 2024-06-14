#include "TetrisBoard.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <glm/ext/vector_float2.hpp>
#include <span>
#include <vector>

#include "../VlEngine/WindowContext.h"

constexpr double MIN_FALL_DELAY = 0.05;
constexpr glm::vec2 UPCOMING_PIECE_PREVIEW_POSITION{17, 11};
constexpr glm::vec2 PIECE_SPAWN_POSTION{7, 0};

constexpr int LEFT_WALL = 2;
constexpr int RIGHT_WALL = 11;
constexpr int BOTTOM_WALL = 19;
constexpr int TOP_WALL = 0;
constexpr int PLAY_AREA_WIDTH = RIGHT_WALL - LEFT_WALL + 1;
constexpr int PLAY_AREA_HEIGHT = BOTTOM_WALL - TOP_WALL + 1;
constexpr int PIECE_BLOCK_COUNT = 4;
constexpr int LINE_CLEARS_PER_LEVEL = 10;
constexpr float WALL_THICKNESS = 0.5F;

constexpr std::array<TetrisBoard::wall_vertex_t, 12> wall_verices = {
    // first triangle
    TetrisBoard::wall_vertex_t{glm::vec2{1.5F, 0.F}, 1.F},
    TetrisBoard::wall_vertex_t{glm::vec2{12.5F, 0.F}, 1.F},
    TetrisBoard::wall_vertex_t{glm::vec2{1.5F, 20.5F}, 1.F},
    // second triangle
    TetrisBoard::wall_vertex_t{glm::vec2{12.5F, 20.5F}, 1.F},
    TetrisBoard::wall_vertex_t{glm::vec2{12.5F, 0.F}, 1.F},
    TetrisBoard::wall_vertex_t{glm::vec2{1.5F, 20.5F}, 1.F},
    // third triangle
    TetrisBoard::wall_vertex_t{glm::vec2{2.F, 0.F}, 0.F},
    TetrisBoard::wall_vertex_t{glm::vec2{12.F, 0.F}, 0.F},
    TetrisBoard::wall_vertex_t{glm::vec2{2.F, 20.F}, 0.F},
    // fourth triangle
    TetrisBoard::wall_vertex_t{glm::vec2{12.F, 20.F}, 0.F},
    TetrisBoard::wall_vertex_t{glm::vec2{12.F, 0.F}, 0.F},
    TetrisBoard::wall_vertex_t{glm::vec2{2.F, 20.F}, 0.F},
};

constexpr TetrisBoard::block_t pieces[][4] =  // NOLINT(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
    {
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

auto is_within_play_area(const TetrisBoard::block_t& block) -> bool {
    return (block.position.x >= LEFT_WALL && block.position.x <= RIGHT_WALL &&
            block.position.y >= TOP_WALL && block.position.y <= BOTTOM_WALL);
}

auto TetrisBoard::get_wall_vertices() -> std::span<const wall_vertex_t> {
    return std::span(wall_verices);
}

auto TetrisBoard::get_current_piece() -> std::span<block_t> {
    return std::span(blocks).subspan(
        blocks.size() - PIECE_BLOCK_COUNT - PIECE_BLOCK_COUNT,
        PIECE_BLOCK_COUNT);
}

auto TetrisBoard::get_upcoming_piece() -> std::span<block_t> {
    return std::span(blocks).subspan(blocks.size() - PIECE_BLOCK_COUNT,
                                     PIECE_BLOCK_COUNT);
}

auto TetrisBoard::get_inactive_blocks() -> std::span<block_t> {
    if (is_piece_locked) {
        return std::span(blocks).subspan(0, blocks.size() - PIECE_BLOCK_COUNT);
    } else {
        return std::span(blocks).subspan(
            0, blocks.size() - PIECE_BLOCK_COUNT - PIECE_BLOCK_COUNT);
    }
}

void TetrisBoard::start_new_game() {
    blocks.clear();

    is_piece_locked = true;
    game_over = false;
    should_start_new_game = false;
    score = 0;
    level = 1;
    lines_cleared_since_level_start = 0;
    fall_delay = 1;

    last_move_down = 0;
    generate_new_upcoming_piece();
}

void TetrisBoard::generate_new_upcoming_piece() {
    const int piece_type = piece_type_distribution(random_generator);

    for (auto block : pieces[piece_type]) {
        blocks.push_back(block);
    }

    for (block_t& block : get_upcoming_piece()) {
        block.position += UPCOMING_PIECE_PREVIEW_POSITION;
    }
}

auto TetrisBoard::is_upcoming_piece_playable() -> bool {
    return !does_movement_collide_with_inactive_blocks(
        get_upcoming_piece(),
        PIECE_SPAWN_POSTION - UPCOMING_PIECE_PREVIEW_POSITION);
}

auto TetrisBoard::does_collide_with_inactive_blocks(
    const std::span<block_t> piece) -> bool {
    return does_movement_collide_with_inactive_blocks(piece, {0, 0});
}

auto TetrisBoard::does_movement_collide_with_inactive_blocks(
    const std::span<block_t> piece, glm::vec2 delta) -> bool {
    return std::ranges::any_of(
               get_inactive_blocks(),
               [&](const block_t& inactive_block) {
                   return std::ranges::any_of(piece, [&](const auto& block) {
                       return block.position + delta == inactive_block.position;
                   });
               }) ||
           std::ranges::any_of(piece, [&](const auto& block) {
               return block.position.x + delta.x < LEFT_WALL ||
                      block.position.x + delta.x > RIGHT_WALL ||
                      block.position.y + delta.y > BOTTOM_WALL;
           });
}

auto TetrisBoard::move_current_piece_if_possible(glm::vec2 delta) -> bool {
    if (!does_movement_collide_with_inactive_blocks(get_current_piece(),
                                                    delta)) {
        for (auto& block : get_current_piece()) {
            block.position += delta;
        }
        return true;
    }
    return false;
}

void TetrisBoard::place_upcoming_piece() {
    for (block_t& block : get_upcoming_piece()) {
        block.position = block.position - UPCOMING_PIECE_PREVIEW_POSITION +
                         PIECE_SPAWN_POSTION;
    }
}

auto TetrisBoard::place_upcoming_piece_if_possible(const double current_time)
    -> bool {
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
    if (input.get_keys().left.action_needed) {
        move += -1;
    }
    if (input.get_keys().right.action_needed) {
        move += 1;
    }
    if (move != 0) {
        move_current_piece_if_possible(glm::ivec2(move, 0));
    }
}

void TetrisBoard::handle_piece_falling(const double current_time) {
    // check if tetromino can move down (on delay (gravity) or on key press)
    // try to move it and if it can't be moved, then deactivate it
    if (current_time - last_move_down > fall_delay ||
        input.get_keys().down.action_needed) {
        if (!move_current_piece_if_possible(glm::ivec2(0, 1))) {
            is_piece_locked = true;
        }

        last_move_down = current_time;
    }
}

void TetrisBoard::handle_piece_rotation() {
    if (input.get_keys().up.action_needed) {
        std::array<block_t, PIECE_BLOCK_COUNT> piece{};
        std::copy(get_current_piece().begin(), get_current_piece().end(),
                  piece.begin());

        // calculate center of rotation
        const glm::vec2 center = piece[0].position;

        // rotate around center
        for (auto& block : piece) {
            const glm::vec2 delta = block.position - center;
            block.position = center + glm::vec2(-delta.y, delta.x);
        }

        // check if tetromino can move to new spot and move it
        if (!does_collide_with_inactive_blocks(piece)) {
            std::copy(piece.begin(), piece.end(), get_current_piece().begin());
        }
    }
}

void TetrisBoard::loop() {
    const double current_time = WindowContext::get_time();

    if (should_start_new_game) {
        start_new_game();
    }

    if (is_piece_locked && !place_upcoming_piece_if_possible(current_time)) {
        return;
    }

    handle_piece_rotation();

    handle_horizontal_movement();

    handle_piece_falling(current_time);

    handle_completed_rows();
}

void TetrisBoard::handle_completed_rows() {
    if (is_piece_locked) {
        // check if any lines are full (only in the play area)
        std::array<int, PLAY_AREA_HEIGHT> count_per_line = {0};
        for (const auto& block : blocks) {
            // only check blocks in the play area
            if (is_within_play_area(block)) {
                count_per_line[static_cast<int>(block.position.y)]++;
            }
        }

        // remove full lines
        const int rows_cleared = static_cast<int>(
            std::erase_if(
                blocks,
                [&](const block_t block) {
                    return is_within_play_area(block) &&
                           count_per_line[static_cast<int>(block.position.y)] ==
                               PLAY_AREA_WIDTH;
                }) /
            PLAY_AREA_WIDTH);

        // calculate how many lines to move the lines down by
        std::array<int, PLAY_AREA_HEIGHT> move_line_down_by{0};
        for (int i = BOTTOM_WALL - 1; i >= TOP_WALL; i--) {
            move_line_down_by[i] =
                move_line_down_by[i + 1] +
                static_cast<int>(count_per_line[i + 1] == PLAY_AREA_WIDTH);
        }

        // move lines down
        for (auto& block : blocks) {
            if (is_within_play_area(block)) {
                block.position.y += static_cast<float>(
                    move_line_down_by[static_cast<int>(block.position.y)]);
            }
        }

        adjust_score(rows_cleared);
    }
}

void TetrisBoard::adjust_score(int rows_cleared) {
    constexpr std::array<int, 6> points_per_row{0, 100, 300, 700, 1500, 3100};
    if (rows_cleared == 0) {
        return;
    }

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

    start_new_level_if_needed();
}

constexpr auto get_fall_delay(int level) -> double {
    constexpr double FALL_DELAY_MULTIPLIER = 0.007;
    constexpr double FALL_DELAY_CONSTANT = 0.8;

    return std::max(
        MIN_FALL_DELAY,
        std::pow(FALL_DELAY_CONSTANT - ((level - 1) * FALL_DELAY_MULTIPLIER),
                 level - 1));
}

void TetrisBoard::start_new_level_if_needed() {
    if (lines_cleared_since_level_start >= LINE_CLEARS_PER_LEVEL) {
        level++;
        lines_cleared_since_level_start -= LINE_CLEARS_PER_LEVEL;
        fall_delay = get_fall_delay(level);
    }
}