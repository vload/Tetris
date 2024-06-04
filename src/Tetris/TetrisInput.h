#ifndef TETRISINPUT_H_
#define TETRISINPUT_H_

#include "../VlEngine/WindowContext.h"

class TetrisInput {
   public:
    struct KeyStatus {
        bool pressed;
        double time_pressed;
        int repeat_count;
        bool action_needed;
        int glfw_key_code;
    };

    struct Keys {
        KeyStatus up{false, 0, 0, false, GLFW_KEY_UP};
        KeyStatus down{false, 0, 0, false, GLFW_KEY_DOWN};
        KeyStatus left{false, 0, 0, false, GLFW_KEY_LEFT};
        KeyStatus right{false, 0, 0, false, GLFW_KEY_RIGHT};
    };

   private:
    WindowContext& window;

    Keys keys;

    void update_key(KeyStatus& key, double current_time);

   public:
    explicit TetrisInput(WindowContext& window);

    void loop();

    // getters / setters
    auto get_keys() -> Keys& { return keys; }

    ~TetrisInput() = default;
    TetrisInput(const TetrisInput&) = delete;
    auto operator=(const TetrisInput&) -> TetrisInput& = delete;
    TetrisInput(TetrisInput&&) = delete;
    auto operator=(TetrisInput&&) -> TetrisInput& = delete;
};

#endif  // TETRISINPUT_H_
