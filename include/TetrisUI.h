#pragma once

#include "TetrisState.h"

class TetrisUI {
   private:
    TetrisState& state;

   public:
    TetrisUI(TetrisState& state);
    ~TetrisUI();

    void loop();

    TetrisUI(const TetrisUI& other) = delete;
    TetrisUI(TetrisUI&& other) = delete;
    TetrisUI& operator=(const TetrisUI& other) = delete;
    TetrisUI& operator=(TetrisUI&& other) noexcept = delete;
};