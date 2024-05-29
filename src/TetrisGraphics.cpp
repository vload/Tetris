#include "TetrisGraphics.h"

TetrisGraphics::TetrisGraphics(WindowContext& window, TetrisBoard& board)
    : window(window),
      board(board),
      block_program("assets/shaders/2d_projection.vert",
                    "assets/shaders/dot_to_square.geom",
                    "assets/shaders/tetris.frag"),
      block_texture("assets/textures/block.png"),
      level_color_texture("assets/textures/level_colors.png") {
    // generate vertex array and buffer objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the VAO and VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // configure the VBO with the square data
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // configure vertex attributes for position and color
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TetrisBoard::block),
                          (void*)offsetof(TetrisBoard::block, position));
    glVertexAttribIPointer(1, 1, GL_INT, sizeof(TetrisBoard::block),
                           (void*)(offsetof(TetrisBoard::block, type)));
    // unbind the VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set the projection matrix based on the window size on window size change
    WindowContext::FramebufferSizeCallback projection_callback =
        [this](GLFWwindow*, int width, int height) {
            float gridWidth = (float)width / block_size;
            float gridHeight = (float)height / block_size;
            projection =
                glm::ortho(0.0f, gridWidth, gridHeight, 0.0f, 0.0f, 100.0f);
        };
    window.add_framebuffer_size_callback(projection_callback);
}

TetrisGraphics::~TetrisGraphics() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void TetrisGraphics::loop() {
    // Rendering
    // ---------------------------------------------------------------------
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // ..:: Drawing code (in render loop) :: ..
    block_program.use();

    // Bind the textures and set their uniforms
    block_texture.bind(0);
    block_program.set_uniform("block_texture", 0);
    level_color_texture.bind(1);
    block_program.set_uniform("level_color_texture", 1);

    // Set the uniforms for the shader
    block_program.set_uniform("projection", projection);
    block_program.set_uniform("level", board.get_level());

    // Bind the VAO and VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Update the VBO with the new data
    glBufferData(GL_ARRAY_BUFFER, sizeof(TetrisBoard::block) * board.get_blocks_size(),
                 board.get_blocks(), GL_DYNAMIC_DRAW);

    // Draw the blocks
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(board.get_blocks_size()));

    // Unbind the VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Unbind the textures
    level_color_texture.unbind(1);
    block_texture.unbind(0);
}