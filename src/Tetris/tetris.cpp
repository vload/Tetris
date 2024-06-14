#include <exception>
#include <iostream>

#include "../VlEngine/WindowContext.h"
#include "TetrisBoard.h"
#include "TetrisGraphics.h"
#include "TetrisInput.h"
#include "TetrisUI.h"

void tetris() {
    // Initialize the game modules
    // ---------------------------
    WindowContext window;
    TetrisInput input(window);
    TetrisBoard board(window, input);
    TetrisGraphics graphics(window, board);
    TetrisUI user_interface(window, board);

    // Render and game loop
    // --------------------
    while (!window.should_close()) {
        TetrisUI::start_frame();

        window.loop();
        input.loop();
        board.loop();
        graphics.loop();
        user_interface.loop();
    }
}

auto main() -> int {  // NOLINT(bugprone-exception-escape)
    try {
        tetris();
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return -1;
    } catch (...) {
        std::cerr << "An unknown error occurred\n";
        return -1;
    }
    return 0;
}