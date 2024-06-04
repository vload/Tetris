#include "TetrisInput.h"

#include <cmath>

#include "../VlEngine/WindowContext.h"

constexpr double DELAY_AFTER_FIRST_KEY_PRESS = 0.25;
constexpr double REPEAT_KEY_DELAY = 0.05;

TetrisInput::TetrisInput(WindowContext& window) : window(window) {}

void TetrisInput::update_key(KeyStatus& key, const double current_time) {
    key.action_needed = false;

    if (window.is_key_pressed(key.glfw_key_code) && !key.pressed) {
        key.time_pressed = current_time;
        key.action_needed = true;
        key.repeat_count = -1;
    }

    key.pressed = window.is_key_pressed(key.glfw_key_code);

    const int repeat_count = static_cast<int>(std::floor(
        (current_time - DELAY_AFTER_FIRST_KEY_PRESS - key.time_pressed) /
        REPEAT_KEY_DELAY));

    if (key.pressed && repeat_count > key.repeat_count) {
        key.action_needed = true;
        key.repeat_count = repeat_count;
    }
}

void TetrisInput::loop() {
    const double current_time = WindowContext::get_time();

    update_key(keys.up, current_time);
    update_key(keys.down, current_time);
    update_key(keys.left, current_time);
    update_key(keys.right, current_time);
}
