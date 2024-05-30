#ifndef TETRISGRAPHICS_H_
#define TETRISGRAPHICS_H_

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
    using block_t = TetrisBoard::block_t;
    VertexBuffer<block_t> block_buffer;
    Texture block_texture;
    Texture level_color_texture;

    Program wall_program;
    using wall_vertex = TetrisBoard::wall_vertex_t;
    VertexBuffer<wall_vertex> wall_buffer;

    glm::mat4 projection{};

    void draw_wall();
    void draw_blocks();

   public:
    TetrisGraphics(WindowContext& window, TetrisBoard& board);

    void loop();
};

#endif  // TETRISGRAPHICS_H_
