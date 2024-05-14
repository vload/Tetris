#include "TetrisLogic.h"

TetrisLogic::TetrisLogic(TetrisState& state) : state(state), squares(state.squares){
    squares = std::vector<square>();
    new_game();
}

void TetrisLogic::new_game() {
    srand(static_cast<unsigned int>(time(NULL)));

    squares.clear();
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
    is_tetromino_active = false;
    state.is_game_over = false;
    state.should_start_new_game = false;

    time_of_last_move_down = 0;
}

bool TetrisLogic::can_move_to(const square sq[4],
                              glm::vec2 delta) {
    size_t last = (is_tetromino_active) ? squares.size() - 4 : squares.size();
    for (int j = 0; j < last; j++) {
        for (int i = 0; i < 4; i++) {
            if (sq[i].position + delta == squares[j].position) {
                return false;
            }
        }
    }
    return true;
}

void TetrisLogic::loop() {
    KeyStatus* keys = state.keys;
    double current_time = glfwGetTime();

    if(state.should_start_new_game){
        new_game();
    }

    // generate new tetromino if necessary
    if (!is_tetromino_active) {
        int tetromino_index = random_int(0, 6);

        if (!can_move_to(tetrominos[tetromino_index])) {
            state.is_game_over = true;
            return;
        }

        for (int i = 0; i < 4; i++)
            squares.push_back(tetrominos[tetromino_index][i]);

        is_tetromino_active = true;

        time_of_last_move_down = current_time;
    }

    if (keys[TetrisDirections::UP].action_needed) {
        square sq[4];
        for (int i = 0; i < 4; i++) {
            sq[i].position = squares[squares.size() - 4 + i].position;
            sq[i].color = squares[squares.size() - 4 + i].color;
        }

        // calculate center of rotation
        glm::vec2 center = sq[0].position;

        // rotate around center
        for (int i = 0; i < 4; i++) {
            glm::vec2 delta = sq[i].position - center;
            sq[i].position = center + glm::vec2(-delta.y, delta.x);
        }

        // check if tetromino can move to new spot and move it
        if (can_move_to(sq)) {
            for (int i = 0; i < 4; i++) {
                squares[squares.size() - 4 + i].position = sq[i].position;
            }
        }
    }

    // process for horizontal movement (left and right)
    int move = 0;
    if (keys[TetrisDirections::LEFT].action_needed) {
        move += -1;
    }
    if (keys[TetrisDirections::RIGHT].action_needed) {
        move += 1;
    }

    // check if tetromino can move to new spot and move it
    if (move != 0 && can_move_to(&squares[squares.size() - 4], glm::ivec2(move, 0))) {
        for (int i = 0; i < 4; i++) {
            squares[squares.size() - 4 + i].position.x += move;
        }
    }

    // check if tetromino can move down (on delay (gravity) or on key press)
    // try to move it and if it can't be moved, then deactivate it
    if (current_time - time_of_last_move_down > state.fall_delay ||
        keys[TetrisDirections::DOWN].action_needed) {
        if (can_move_to(&squares[squares.size() - 4], glm::ivec2(0, 1))) {
            for (int i = 0; i < 4; i++) {
                squares[squares.size() - 4 + i].position.y += 1;
            }
        } else {
            is_tetromino_active = false;
        }

        time_of_last_move_down = current_time;
    }

    // clear lines if necessary
    if (!is_tetromino_active) {
        // check if any lines are full (only in the play area)
        int count_per_line[20] = {0};
        for (int j = 0; j < squares.size(); j++) {
            if (squares[j].position.x > 1 && squares[j].position.x < 12 &&
                squares[j].position.y >= 0 && squares[j].position.y < 20) {
                count_per_line[(int)squares[j].position.y]++;
            }
        }

        // remove full lines
        std::erase_if(squares, [&](square s) {
            return s.position.x > 1 && s.position.x < 12 && s.position.y > 0 &&
                   s.position.y < 20 && count_per_line[(int)s.position.y] == 10;
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
                    move_line_down_by[(int)squares[i].position.y];
            }
        }
    }
}