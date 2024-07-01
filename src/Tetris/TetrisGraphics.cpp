#include "TetrisGraphics.h"

#include <glad/glad.h>
// This order is important
#include <GLFW/glfw3.h>

#include <glm/ext/matrix_clip_space.hpp>

#include "../VlEngine/Bind.h"
#include "../VlEngine/WindowContext.h"
#include "TetrisBoard.h"
constexpr float BLOCK_SIZE = 48.F;

TetrisGraphics::TetrisGraphics(WindowContext& window, TetrisBoard& board)
    : window(window),
      board(board),
      block_program("assets/shaders/tetris_block.vert",
                    "assets/shaders/tetris_block.geom",
                    "assets/shaders/tetris_block.frag"),
      wall_program("assets/shaders/tetris_wall.vert",
                   "assets/shaders/tetris_wall.frag"),
      block_texture(GL_TEXTURE_2D, 0),
      level_color_texture(GL_TEXTURE_2D, 1) {
    WindowContext::FramebufferSizeCallback projection_matrix_callback =
        [this](GLFWwindow*, int width, int height) {
            const float gridWidth = static_cast<float>(width) / BLOCK_SIZE;
            const float gridHeight = static_cast<float>(height) / BLOCK_SIZE;
            projection =
                glm::ortho(0.F, gridWidth, gridHeight, 0.F, 0.F, 100.F);
        };
    window.add_framebuffer_size_callback(projection_matrix_callback);

    // Set the attribute pointers for the block buffer
    block_vao.add_attribute(block_buffer, 0, &block_t::position);
    block_vao.add_attribute(block_buffer, 1, &block_t::type);

    // Set the attribute pointers for the wall buffer
    wall_vao.add_attribute(wall_buffer, 0, &wall_vertex_t::position);
    wall_vao.add_attribute(wall_buffer, 1, &wall_vertex_t::color);
    wall_buffer.copy_data(TetrisBoard::get_wall_vertices(), GL_STATIC_DRAW);

    glClearColor(0.F, 0.F, 0.F, 1.F);

    block_texture.load_image("assets/textures/block.png");
    level_color_texture.load_image("assets/textures/level_colors.png");
    block_texture.set_nearest();
    level_color_texture.set_nearest();
}

void TetrisGraphics::draw_wall() {
    auto program_bind = Bind(wall_program);
    auto vao_bind = Bind(wall_vao);
    auto buffer_bind = Bind(wall_buffer, GL_ARRAY_BUFFER);

    // Set the uniforms for the shader
    wall_program.set_uniform("projection", projection);

    // Draw the walls
    glDrawArrays(GL_TRIANGLES, 0,
                 static_cast<GLsizei>(TetrisBoard::get_wall_vertices().size()));
}

void TetrisGraphics::draw_blocks() {
    auto program_bind = Bind(block_program);
    auto block_texture_bind = Bind(block_texture);
    auto colors_texture_bind = Bind(level_color_texture);
    auto vao_bind = Bind(block_vao);
    auto buffer_bind = Bind(block_buffer, GL_ARRAY_BUFFER);

    // Set the texture uniforms
    block_program.set_uniform("block_texture", 0);
    block_program.set_uniform("level_color_texture", 1);

    // Set the uniforms for the shader
    block_program.set_uniform("projection", projection);
    block_program.set_uniform("level", board.get_level());

    // Copy the block data to the buffer
    block_buffer.copy_data(board.get_blocks(), GL_DYNAMIC_DRAW);

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