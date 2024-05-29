#include "TetrisBoard.h"
#include "TetrisGraphics.h"
#include "TetrisInput.h"
#include "TetrisUI.h"
#include "WindowContext.h"

void game() {
    // Initialize the game modules
    // ---------------------------
    WindowContext window;
    TetrisInput input(window);
    TetrisBoard board(window, input);
    TetrisGraphics graphics(window, board);
    TetrisUI ui(window, board);

    // Render and game loop
    // --------------------
    while (!window.should_close()) {
        ui.start_frame();

        window.loop();
        input.loop();
        board.loop();
        graphics.loop();
        ui.loop();
    }
}

int main() {
    try {
        game();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
