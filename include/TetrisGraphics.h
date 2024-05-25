#pragma once

#include "Program.h"
#include "Texture.h"
// include order matters
#include "util.h"

class TetrisGraphics {
   private:
    Program program;
    Texture block_texture;
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
