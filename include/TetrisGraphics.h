#pragma once

#include "Program.h"
#include "Texture.h"
// include order matters
#include "TetrisBoard.h"
#include "WindowContext.h"

class TetrisGraphics {
   private:
    WindowContext& window;
    TetrisBoard& board;

    Program block_program;
    Texture block_texture;
    Texture level_color_texture;
    unsigned int VAO, VBO;

    int block_size = 48;
    glm::mat4 projection;

   public:
    TetrisGraphics(WindowContext& window, TetrisBoard& board);
    ~TetrisGraphics();

    void loop();

    TetrisGraphics(const TetrisGraphics& other) = delete;
    TetrisGraphics(TetrisGraphics&& other) = delete;
    TetrisGraphics& operator=(const TetrisGraphics& other) = delete;
    TetrisGraphics& operator=(TetrisGraphics&& other) noexcept = delete;
};
