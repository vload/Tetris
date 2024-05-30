#include "TetrisGraphics.h"

#include "Bind.h"
#include "TetrisBoard.h"
#include "WindowContext.h"

constexpr float BLOCK_SIZE = 48.F;

TetrisGraphics::TetrisGraphics(WindowContext& window, TetrisBoard& board)
    : window(window),
      board(board),
      block_program("assets/shaders/tetris_block.vert",
                    "assets/shaders/tetris_block.geom",
                    "assets/shaders/tetris_block.frag"),
      wall_program("assets/shaders/tetris_wall.vert",
                   "assets/shaders/tetris_wall.frag"),
      block_texture("assets/textures/block.png", 0),
      level_color_texture("assets/textures/level_colors.png", 1) {
    // Set the projection matrix based on the window size on window size change
    WindowContext::FramebufferSizeCallback projection_callback =
        [this](GLFWwindow*, int width, int height) { // NOLINT
            const float gridWidth = static_cast<float>(width) / BLOCK_SIZE;
            const float gridHeight = static_cast<float>(height) / BLOCK_SIZE;
            projection =
                glm::ortho(0.F, gridWidth, gridHeight, 0.F, 0.F, 100.F);
        };
    window.add_framebuffer_size_callback(projection_callback);

    // Set the attribute pointers for the block buffer
    auto block_bind = Bind(block_buffer);
    block_buffer.add_attribute(0, &block_t::position);
    block_buffer.add_attribute(1, &block_t::type);

    // Set the attribute pointers for the wall buffer
    auto wall_bind = Bind(wall_buffer);
    wall_buffer.add_attribute(0, &wall_vertex::position);
    wall_buffer.add_attribute(1, &wall_vertex::color);
    wall_buffer.copy_static_data(TetrisBoard::get_wall_vertices());

    glClearColor(0.F, 0.F, 0.F, 1.F);
}

void TetrisGraphics::draw_wall() {
    auto program_bind = Bind(wall_program);
    auto buffer_bind = Bind(wall_buffer);

    // Set the uniforms for the shader
    wall_program.set_uniform("projection", projection);

    // Draw the walls
    glDrawArrays(GL_TRIANGLES, 0,
                 static_cast<GLsizei>(TetrisBoard::get_wall_vertices().size()));
}

void TetrisGraphics::draw_blocks() {
    auto program_bind = Bind(block_program);
    auto texture_bind_0 = Bind(block_texture);
    auto texture_bind_1 = Bind(level_color_texture);
    auto buffer_bind = Bind(block_buffer);

    // Set the texture uniforms
    block_program.set_uniform("block_texture", 0);
    block_program.set_uniform("level_color_texture", 1);

    // Set the uniforms for the shader
    block_program.set_uniform("projection", projection);
    block_program.set_uniform("level", board.get_level());

    // Copy the block data to the buffer
    block_buffer.copy_dynamic_data(board.get_blocks());

    // Draw the blocks
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(board.get_blocks().size()));
}

void TetrisGraphics::loop() {
    // Rendering
    // ---------------------------------------------------------------------
    glClear(GL_COLOR_BUFFER_BIT);

    draw_wall();

    draw_blocks();
}