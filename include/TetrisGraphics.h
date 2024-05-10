#pragma once

#include "Program.h"
// order matters
#include "TetrisState.h"

class TetrisGraphics {
   private:
    Program program;
    TetrisState& state;
    unsigned int VAO, VBO;

   public:
    TetrisGraphics(TetrisState& state);
    ~TetrisGraphics();

    void loop();

    TetrisGraphics(const TetrisGraphics& other) = delete;
    TetrisGraphics(TetrisGraphics&& other) = delete;
    TetrisGraphics& operator=(const TetrisGraphics& other) = delete;
    TetrisGraphics& operator=(TetrisGraphics&& other) noexcept = delete;
};
