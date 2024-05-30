#pragma once

#include "Program.h"
#include "Texture.h"
// include order matters
#include "TetrisBoard.h"
#include "VertexBuffer.h"
#include "WindowContext.h"

class TetrisGraphics {
   private:
    WindowContext& window;
    TetrisBoard& board;

    Program block_program;
    using block = TetrisBoard::block;
    VertexBuffer<block> block_buffer;
    Texture block_texture;
    Texture level_color_texture;

    Program wall_program;
    using wall_vertex = TetrisBoard::wall_vertex;
    VertexBuffer<wall_vertex> wall_buffer;

    int block_size = 48;
    glm::mat4 projection;

    void draw_wall();
    void draw_blocks();

   public:
    TetrisGraphics(WindowContext& window, TetrisBoard& board);

    void loop();
};
