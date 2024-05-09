#include <iostream>  // for debugging purposes

#include "Input.h"
#include "Program.h"
#include "Tetris.h"
#include "WindowContext.h"
#include "util.h"

int main() {
    // Setup window, OpenGL pipeline and vertex array + buffer for the squares
    // -----------------------------------------------------------------------
    WindowContext window = WindowContext();
    Program program =
        Program("shaders/2d_projection.vert", "shaders/dot_to_square.geom",
                "shaders/static_color.frag");
    // generate vertex array and buffer objects
    unsigned int VAO, VBO;
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

    // Setup input
    // -----------
    TetrisInput input = TetrisInput(window);

    // Setup game elements
    // -------------------
    int grid_step = 50;
    Tetris tetris = Tetris(input);

    // Render and game loop
    // --------------------
    while (!window.should_close()) {
        // check and call events
        glfwPollEvents();

        // ImGui
        // ---------------------------------------------------------------------
        // start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // imgui menu
        constexpr double ONE = 1.0;
        constexpr double ZERO = 0.0;
        ImGui::SliderScalar("delay1", ImGuiDataType_Double, &delay1, &ZERO,
                            &ONE, "%.3f s");
        ImGui::SliderScalar("delay2", ImGuiDataType_Double, &delay2, &ZERO,
                            &ONE, "%.3f s");
        ImGui::SliderScalar("fall delay", ImGuiDataType_Double, &fall_delay,
                            &ZERO, &ONE, "%.3f s");
        if (ImGui::Button("New Game")) {
            tetris.new_game();
        }
        if (tetris.is_game_over()) {
            ImGui::Text("Game Over!");
        }
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);

        /// Input
        // ---------------------------------------------------------------------
        input.process();

        // Game logic
        // ---------------------------------------------------------------------
        tetris.update();

        // Rendering
        // ---------------------------------------------------------------------
        // Compute the number of grid cells in each axis
        float gridWidth = (float)window.get_width() / grid_step;
        float gridHeight = (float)window.get_height() / grid_step;

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
        glBufferData(GL_ARRAY_BUFFER, sizeof(square) * tetris.size(),
                     tetris.data(), GL_DYNAMIC_DRAW);
        // Draw the squares
        glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(tetris.size()));
        // Unbind the VAO and VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // render imgui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // swap the buffers
        window.swap_buffers();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}
