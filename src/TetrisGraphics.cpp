#include "TetrisGraphics.h"

TetrisGraphics::TetrisGraphics(TetrisState& state)
    : state(state),
      program("shaders/2d_projection.vert", "shaders/dot_to_square.geom",
              "shaders/static_color.frag") {
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(square),
                          (void*)offsetof(square, position));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(square),
                          (void*)(offsetof(square, color)));
    // unbind the VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TetrisGraphics::~TetrisGraphics() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void TetrisGraphics::loop() {
    // Rendering
    // ---------------------------------------------------------------------
    // Compute the number of grid cells in each axis
    float gridWidth = (float)state.width / state.grid_step;
    float gridHeight = (float)state.height / state.grid_step;

    // Create the orthographic projection matrix
    glm::mat4 projection =
        glm::ortho(0.0f, gridWidth, gridHeight, 0.0f, 0.0f, 100.0f);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // ..:: Drawing code (in render loop) :: ..
    program.use();
    program.set_uniform("projection", projection);

    // Bind the VAO and VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Update the VBO with the new data
    glBufferData(GL_ARRAY_BUFFER, sizeof(square) * state.squares.size(),
                 state.squares.data(), GL_DYNAMIC_DRAW);
    // Draw the squares
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(state.squares.size()));
    // Unbind the VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}