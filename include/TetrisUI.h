#ifndef TETRISUI_H_
#define TETRISUI_H_

#include "TetrisBoard.h"
#include "WindowContext.h"

class TetrisUI {
   private:
    WindowContext& window;
    TetrisBoard& board;

   public:
    TetrisUI(WindowContext& window, TetrisBoard& board);
    ~TetrisUI();

    static void start_frame();

    void loop();

    TetrisUI(const TetrisUI& other) = delete;
    TetrisUI(TetrisUI&& other) = delete;
    auto operator=(const TetrisUI& other) -> TetrisUI& = delete;
    auto operator=(TetrisUI&& other) noexcept -> TetrisUI& = delete;
};

#endif  // TETRISUI_H_
