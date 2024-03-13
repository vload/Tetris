#include <iostream>  // for debugging purposes

#include "Program.h"
#include "WindowContext.h"
#include "util.h"

float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

void process_input(WindowContext& window);

int main() {
    WindowContext window = WindowContext();

    Program program = Program("shaders/basic.vert", "shaders/basic.frag");

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    // ..:: Initialization code :: ..
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);

    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 4. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);   
    
    float grey = 0;
    
    // render loop
    // -----------
    while (!window.should_close()) {
        // imgui new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui::ShowDemoWindow(); // Show demo window! :)
        ImGui::SliderFloat("grey", &grey, 0.0f, 1.0f, "grey = %.4f");

        // input
        // -----
        process_input(window);

        // render
        // ------
        glClearColor(grey, grey, grey, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ..:: Drawing code (in render loop) :: ..
        program.use();

        float greenValue = ((float) sin(glfwGetTime()) / 2.0f) + 0.5f;
        program.set_uniform("color", glm::vec4(0.0f, greenValue, 0.0f, 1.0f));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
    glDeleteBuffers(1, &EBO);

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void process_input(WindowContext& window) {
    if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS)
        window.set_should_close(true);
}
