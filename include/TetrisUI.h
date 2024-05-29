#pragma once

#include "TetrisBoard.h"
#include "WindowContext.h"

class TetrisUI {
   private:
    WindowContext& window;
    TetrisBoard& board;

   public:
    TetrisUI(WindowContext& window, TetrisBoard& board);
    ~TetrisUI();

    void start_frame();

    void loop();

    TetrisUI(const TetrisUI& other) = delete;
    TetrisUI(TetrisUI&& other) = delete;
    TetrisUI& operator=(const TetrisUI& other) = delete;
    TetrisUI& operator=(TetrisUI&& other) noexcept = delete;
};