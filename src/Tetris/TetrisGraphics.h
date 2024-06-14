#ifndef TETRISGRAPHICS_H_
#define TETRISGRAPHICS_H_

#include <glm/glm.hpp>

#include "../VlEngine/BufferObject.h"
#include "../VlEngine/Program.h"
#include "../VlEngine/Texture.h"
#include "../VlEngine/VertexArrayObject.h"
#include "../VlEngine/WindowContext.h"
#include "TetrisBoard.h"

class TetrisGraphics {
   private:
    WindowContext& window;
    TetrisBoard& board;

    Program block_program;
    using block_t = TetrisBoard::block_t;
    VertexArrayObject block_vao;
    BufferObject<block_t> block_buffer;
    Texture block_texture;
    Texture level_color_texture;

    Program wall_program;
    using wall_vertex_t = TetrisBoard::wall_vertex_t;
    VertexArrayObject wall_vao;
    BufferObject<wall_vertex_t> wall_buffer;

    glm::mat4 projection{};

    void draw_wall();
    void draw_blocks();

   public:
    TetrisGraphics(WindowContext& window, TetrisBoard& board);

    void loop();
};

#endif  // TETRISGRAPHICS_H_
