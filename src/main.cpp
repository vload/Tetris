#include <iostream>  // for debugging purposes

#include "Program.h"
#include "Tetris.h"
#include "WindowContext.h"
#include "Input.h"
#include "util.h"

int main() {
    // Setup window, OpenGL pipeline and vertex array + buffer for the squares
    // -----------------------------------------------------------------------
    WindowContext window = WindowContext();
    Program program = Program("shaders/transform.vert",
                              "shaders/dotsquare.geom", "shaders/uni.frag");
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // configure vertex attributes for position and color
    glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, sizeof(square),
                          (void*)(offsetof(square, position)));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(square),
                          (void*)(offsetof(square, color)));

    // Setup input
    // -----------
    TetrisInput input = TetrisInput(window);

    // Setup game elements
    // -------------------
    int grid_size = 200;
    int grid_step = 50;
    float tick_duration = 1.0f / 20.0f;
    double last_tick = floor(window.get_time() / tick_duration);
    Tetris tetris = Tetris(input);

    // Render and game loop
    // --------------------
    while (!window.should_close()) {
        // Game logic
        // ---------------------------------------------------------------------
        double current_tick = floor(window.get_time() / tick_duration);

        // simulate game logic for missing ticks
        while (last_tick < current_tick) {
            tetris.do_tick();
            last_tick += 1.0;
        }

        // Rendering
        // ---------------------------------------------------------------------
        // ImGui new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // imgui menu
        ImGui::SliderInt("grid step", &grid_step, 0, 100, "%d px");
        ImGui::SliderFloat("tick duration", &tick_duration, 0.005f, 0.2f,
                           "%.3f s");
        if(tetris.is_game_over()) {
            ImGui::Text("Game Over!");
        }
        // ImGui::SliderInt("grid size", &grid_size, 0, 1000, "%d squares");
        // ImGui::ShowDemoWindow();
        // ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        // input
        input.process();

        // render
        // ------

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

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(square) * tetris.size(),
                     tetris.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(tetris.size()));
        glBindVertexArray(0);

        // render imgui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // check and call events and swap the buffers
        window.swap_buffers();
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}
