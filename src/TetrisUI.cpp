#include "TetrisUI.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include <format>
#include <string>

#include "TetrisBoard.h"
#include "WindowContext.h"

TetrisUI::TetrisUI(WindowContext& window, TetrisBoard& board)
    : window(window), board(board) {
    constexpr auto config_flags = ImGuiConfigFlags_DockingEnable;

    // Setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& imgui_io = ImGui::GetIO();
    imgui_io.ConfigFlags |= config_flags;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
    ImGui_ImplOpenGL3_Init();
}

TetrisUI::~TetrisUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void TetrisUI::start_frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void TetrisUI::loop() {
    constexpr auto window_flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoBackground;
    constexpr auto window_position = ImVec2(700, 100);
    constexpr auto window_size = ImVec2(600, 300);

    // create a transparent window
    ImGui::SetNextWindowBgAlpha(0.F);
    ImGui::Begin("Tetris", nullptr, window_flags);
    ImGui::SetWindowPos(window_position, ImGuiCond_Always);
    ImGui::SetWindowSize(window_size, ImGuiCond_Always);
    ImGui::SetWindowFontScale(2);

    // imgui menu
    if (ImGui::Button("New Game")) {
        board.initiate_new_game();
    }
    if (board.is_game_over()) {
        const std::string game_over_text = "Game Over ";
        ImGui::TextUnformatted(&game_over_text.front(), &game_over_text.back());
    }
    const std::string score_text = std::format("Score: {} ", board.get_score());
    ImGui::TextUnformatted(&score_text.front(), &score_text.back());

    const std::string level_text = std::format("Level: {} ", board.get_level());
    ImGui::TextUnformatted(&level_text.front(), &level_text.back());

    ImGui::End();

    // render imgui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}