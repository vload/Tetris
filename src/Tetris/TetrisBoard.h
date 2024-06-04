#ifndef TETRISBOARD_H_
#define TETRISBOARD_H_

#include <chrono>
#include <random>
#include <span>

#include "../VlEngine/WindowContext.h"
#include "TetrisInput.h"

constexpr int PIECE_TYPE_COUNT = 7;

class TetrisBoard {
   public:
    struct block_t {
        glm::vec2 position;
        int type;
    };

    struct wall_vertex_t {
        glm::vec2 position;
        float color;
    };

   private:
    WindowContext& window;
    TetrisInput& input;

    std::vector<block_t> blocks;
    bool previous_clear_was_tetris = false;
    bool should_start_new_game = true;
    bool game_over = false;
    int level = 1;
    int lines_cleared_since_level_start = 0;
    int score = 0;
    double last_move_down = 0;
    double fall_delay = 1;
    bool is_piece_locked = false;

    std::mt19937 random_generator = std::mt19937(static_cast<unsigned int>(
        std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<> piece_type_distribution{
        0, PIECE_TYPE_COUNT - 1};

    void start_new_game();
    void start_new_level_if_needed();

    auto does_collide_with_inactive_blocks(std::span<block_t> piece) -> bool;
    auto does_movement_collide_with_inactive_blocks(std::span<block_t> piece,
                                                    glm::vec2 delta) -> bool;
    auto move_current_piece_if_possible(glm::vec2 delta) -> bool;
    auto is_upcoming_piece_playable() -> bool;
    void place_upcoming_piece();
    void generate_new_upcoming_piece();
    auto place_upcoming_piece_if_possible(double current_time) -> bool;

    void handle_horizontal_movement();
    void handle_piece_rotation();
    void handle_piece_falling(double current_time);
    void handle_completed_rows();

    auto get_current_piece() -> std::span<block_t>;
    auto get_upcoming_piece() -> std::span<block_t>;
    auto get_inactive_blocks() -> std::span<block_t>;

    void adjust_score(int rows_cleared);

   public:
    TetrisBoard(WindowContext& window, TetrisInput& input)
        : window(window), input(input) {}

    void loop();

    // getters / setters
    auto get_blocks() -> std::span<const block_t> { return std::span(blocks); }
    static auto get_wall_vertices() -> std::span<const wall_vertex_t>;
    [[nodiscard]] auto is_game_over() const -> bool { return game_over; }
    [[nodiscard]] auto get_level() const -> int { return level; }
    [[nodiscard]] auto get_score() const -> int { return score; }
    void initiate_new_game() { should_start_new_game = true; }

    ~TetrisBoard() = default;
    TetrisBoard(const TetrisBoard&) = delete;
    TetrisBoard(TetrisBoard&&) = delete;
    auto operator=(const TetrisBoard&) -> TetrisBoard& = delete;
    auto operator=(TetrisBoard&&) -> TetrisBoard& = delete;
};

#endif  // TETRISBOARD_H_
