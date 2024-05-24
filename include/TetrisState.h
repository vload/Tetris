#pragma once

#include "util.h"

struct block {
    glm::vec2 position;
    glm::vec4 color;
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
        {glm::ivec2(0, 0), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
        {glm::ivec2(-1, 0), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
        {glm::ivec2(-1, 1), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
        {glm::ivec2(0, 1), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
    },
    {
        // I
        {glm::ivec2(0, 0), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
        {glm::ivec2(-1, 0), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
        {glm::ivec2(1, 0), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
        {glm::ivec2(-2, 0), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
    },
    {
        // T
        {glm::ivec2(0, 0), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)},
        {glm::ivec2(-1, 0), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)},
        {glm::ivec2(1, 0), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)},
        {glm::ivec2(0, 1), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)},
    },
    {
        // L
        {glm::ivec2(0, 0), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)},
        {glm::ivec2(-1, 0), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)},
        {glm::ivec2(1, 0), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)},
        {glm::ivec2(-1, 1), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)},
    },
    {
        // J
        {glm::ivec2(0, 0), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)},
        {glm::ivec2(-1, 0), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)},
        {glm::ivec2(1, 0), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)},
        {glm::ivec2(1, 1), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)},
    },
    {
        // Z
        {glm::ivec2(0, 0), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)},
        {glm::ivec2(-1, 0), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)},
        {glm::ivec2(0, 1), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)},
        {glm::ivec2(1, 1), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)},
    },
    {
        // S
        {glm::ivec2(0, 0), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
        {glm::ivec2(1, 0), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
        {glm::ivec2(0, 1), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
        {glm::ivec2(-1, 1), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
    },
};

struct TetrisState {
    // settings
    double fall_delay = 0.5;
    double delay_after_first_key_press = 0.25;
    double repeat_key_delay = 0.05;
    double current_time = 0;

    GLFWwindow* window = nullptr;
    int width = 1100;
    int height = 1100;
    int block_size = 50;

    bool should_start_new_game = false;
    bool is_game_over = false;
    std::vector<block> blocks = {};
    int score = 0;
    bool previous_clear_was_tetris = false;

    std::array<KeyStatus, TetrisDirections::_COUNT> keys = {0};
};