#include "TetrisUI.h"

// imgui
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

TetrisUI::TetrisUI(TetrisState& state) : state(state) {
    // Setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // io.ConfigFlags |=
    //     ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_DockingEnable;  // IF using Docking Branch
    // io.ConfigDockingWithShift = true;    // IF using Docking Branch
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(
        state.window,
        true);  // Second param install_callback=true will install
                // GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}

TetrisUI::~TetrisUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void TetrisUI::loop() {
    // start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // create a transparent window
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::Begin("Tetris", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoBackground);
    ImGui::SetWindowPos(ImVec2(709, 73), ImGuiCond_Always);
    ImGui::SetWindowSize(ImVec2(390, 181), ImGuiCond_Always);
    ImGui::SetWindowFontScale(2);

    // imgui menu
    if (ImGui::Button("New Game")) {
        state.should_start_new_game = true;
    }
    if (state.is_game_over) {
        ImGui::Text("Game Over!");
    }
    ImGui::Text("Score: %d", state.score);

    ImGui::End();

    // render imgui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}