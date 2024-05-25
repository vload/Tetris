#pragma once

#include "util.h"

struct block {
    glm::vec2 position;
    int32_t type;
};

constexpr enum TetrisDirections {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3,
    _COUNT = 4,
};

constexpr int TetrisKeyNamesGLFW[] = {
    GLFW_KEY_UP,
    GLFW_KEY_DOWN,
    GLFW_KEY_LEFT,
    GLFW_KEY_RIGHT,
};

struct KeyStatus {
    bool pressed;
    double time_pressed;
    double prev_x;
    bool action_needed;
};

constexpr block tetrominos[][4] = {
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

struct TetrisState {
    GLFWwindow* window = nullptr;
    int width = 1100;
    int height = 1100;
    int block_size = 48;

    bool should_start_new_game = true;
    bool is_game_over = false;
    std::vector<block> blocks = {};
    int score = 0;
    bool previous_clear_was_tetris = false;
    bool is_scene_dirty = false;
    double current_time = 0;
    int level = 1;
    int clears = 0;

    std::array<KeyStatus, TetrisDirections::_COUNT> keys = {0};
};