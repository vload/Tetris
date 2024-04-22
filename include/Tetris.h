#pragma once

#include "TetrisInput.h"
#include "util.h"

// settings
double key_repeat_delay = 0.2;
double fall_delay = 0.5;

struct square {
    glm::ivec2 position;
    glm::vec4 color;
};

constexpr square tetrominos[][4] = {
    {
        // O
        {glm::ivec2(7, 0), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
        {glm::ivec2(6, 0), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
        {glm::ivec2(6, 1), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
        {glm::ivec2(7, 1), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
    },
    {
        // I
        {glm::ivec2(7, 0), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
        {glm::ivec2(6, 0), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
        {glm::ivec2(8, 0), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
        {glm::ivec2(5, 0), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
    },
    {
        // T
        {glm::ivec2(7, 0), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)},
        {glm::ivec2(6, 0), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)},
        {glm::ivec2(8, 0), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)},
        {glm::ivec2(7, 1), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)},
    },
    {
        // L
        {glm::ivec2(7, 0), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)},
        {glm::ivec2(6, 0), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)},
        {glm::ivec2(8, 0), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)},
        {glm::ivec2(6, 1), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)},
    },
    {
        // J
        {glm::ivec2(7, 0), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)},
        {glm::ivec2(6, 0), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)},
        {glm::ivec2(8, 0), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)},
        {glm::ivec2(8, 1), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)},
    },
    {
        // Z
        {glm::ivec2(7, 0), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)},
        {glm::ivec2(6, 0), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)},
        {glm::ivec2(7, 1), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)},
        {glm::ivec2(8, 1), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)},
    },
    {
        // S
        {glm::ivec2(7, 0), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
        {glm::ivec2(8, 0), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
        {glm::ivec2(7, 1), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
        {glm::ivec2(6, 1), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
    },
};

class Tetris {
   private:
    // active_tetromino is the last 4 elements of squares
    std::vector<square> squares;
    bool is_tetromino_active = false;
    bool game_over = false;
    TetrisInput input;
    double time_since_last_move[TetrisDirections::_COUNT] = {0};

   public:
    Tetris(TetrisInput& input) : input(input) {
        squares = std::vector<square>();
        // put up walls
        for (int i = -5; i < 20; i++) {
            squares.push_back(
                {glm::ivec2(1, i), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)});
            squares.push_back(
                {glm::ivec2(12, i), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)});
        }
        for (int i = 1; i < 13; i++) {
            squares.push_back(
                {glm::ivec2(i, 20), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)});
        }
    }

    bool is_game_over() { return game_over; }

    bool can_move_to(const square sq[4], glm::ivec2 delta = glm::ivec2(0, 0)) {
        size_t last =
            (is_tetromino_active) ? squares.size() - 4 : squares.size();
        for (int j = 0; j < last; j++) {
            for (int i = 0; i < 4; i++) {
                if (sq[i].position + delta == squares[j].position) {
                    return false;
                }
            }
        }
        return true;
    }

    void update(double delta_t) {
        // generate new tetromino if necessary
        if (!is_tetromino_active) {
            int tetromino_index = random_int(0, 6);

            if (!can_move_to(tetrominos[tetromino_index])) {
                game_over = true;
                return;
            }

            for (int i = 0; i < 4; i++)
                squares.push_back(tetrominos[tetromino_index][i]);

            is_tetromino_active = true;

            time_since_last_move[TetrisDirections::UP] = 0;
            time_since_last_move[TetrisDirections::DOWN] = 0;
            time_since_last_move[TetrisDirections::LEFT] = 0;
            time_since_last_move[TetrisDirections::RIGHT] = 0;
        }

        if (input.is_key_pressed(GLFW_KEY_UP) &&
            time_since_last_move[TetrisDirections::UP] > key_repeat_delay) {
            square sq[4];
            for (int i = 0; i < 4; i++) {
                sq[i].position = squares[squares.size() - 4 + i].position;
                sq[i].color = squares[squares.size() - 4 + i].color;
            }

            // calculate center of rotation
            glm::ivec2 center = sq[0].position;

            // rotate around center
            for (int i = 0; i < 4; i++) {
                glm::ivec2 delta = sq[i].position - center;
                sq[i].position = center + glm::ivec2(-delta.y, delta.x);
            }

            // check if tetromino can move to new spot and move it
            if (can_move_to(sq)) {
                for (int i = 0; i < 4; i++) {
                    squares[squares.size() - 4 + i].position = sq[i].position;
                }
            }

            time_since_last_move[TetrisDirections::UP] = 0;
        }

        // process input (down, left, right(check spot) space?)
        glm::ivec2 move(0, 0);
        if (input.is_key_pressed(GLFW_KEY_LEFT) &&
            time_since_last_move[TetrisDirections::LEFT] > key_repeat_delay) {
            move.x += -1;
            time_since_last_move[TetrisDirections::LEFT] = 0;
        }
        if (input.is_key_pressed(GLFW_KEY_RIGHT) &&
            time_since_last_move[TetrisDirections::RIGHT] > key_repeat_delay) {
            move.x += 1;
            time_since_last_move[TetrisDirections::RIGHT] = 0;
        }
        if (input.is_key_pressed(GLFW_KEY_DOWN) &&
            time_since_last_move[TetrisDirections::DOWN] > key_repeat_delay) {
            move.y += 1;
            time_since_last_move[TetrisDirections::DOWN] = 0;
        }

        // check if tetromino can move to new spot and move it
        if (can_move_to(&squares[squares.size() - 4], move)) {
            for (int i = 0; i < 4; i++) {
                squares[squares.size() - 4 + i].position += move;
            }
        }

        // check if tetromino can not move down (every n ticks), move it and
        // deactivate it
        if (time_since_last_move[TetrisDirections::DOWN] > fall_delay) {
            if (can_move_to(&squares[squares.size() - 4], glm::ivec2(0, 1))) {
                for (int i = 0; i < 4; i++) {
                    squares[squares.size() - 4 + i].position.y += 1;
                }
            } else {
                is_tetromino_active = false;
            }

            time_since_last_move[TetrisDirections::DOWN] = 0;
        }

        // clear lines if necessary
        if (!is_tetromino_active) {
            // check if any lines are full (only in the play area)
            int count_per_line[20] = {0};
            for (int j = 0; j < squares.size(); j++) {
                if (squares[j].position.x > 1 && squares[j].position.x < 12 &&
                    squares[j].position.y >= 0 && squares[j].position.y < 20) {
                    count_per_line[squares[j].position.y]++;
                }
            }

            // remove full lines
            std::erase_if(squares, [&](square s) {
                return s.position.x > 1 && s.position.x < 12 &&
                       s.position.y > 0 && s.position.y < 20 &&
                       count_per_line[s.position.y] == 10;
            });

            // calculate how many lines to move the lines down by
            int move_line_down_by[20] = {0};
            for (int i = 18; i >= 0; i--) {
                move_line_down_by[i] =
                    move_line_down_by[i + 1] + (count_per_line[i + 1] == 10);
            }

            // move lines down
            for (int i = 0; i < squares.size(); i++) {
                if (squares[i].position.x > 1 && squares[i].position.x < 12 &&
                    squares[i].position.y < 20) {
                    squares[i].position.y +=
                        move_line_down_by[squares[i].position.y];
                }
            }
        }

        time_since_last_move[TetrisDirections::UP] += delta_t;
        time_since_last_move[TetrisDirections::DOWN] += delta_t;
        time_since_last_move[TetrisDirections::LEFT] += delta_t;
        time_since_last_move[TetrisDirections::RIGHT] += delta_t;
    }

    size_t size() { return squares.size(); }
    square* data() { return squares.data(); }
};