#include "TetrisState.h"
// order matters
#include "TetrisGraphics.h"
#include "TetrisInput.h"
#include "TetrisLogic.h"
#include "TetrisUI.h"
#include "WindowContext.h"

void game() {
    // Setup the and state and initialize the game
    TetrisState state;
    WindowContext window(state);
    TetrisInput input(state);
    TetrisLogic logic(state);
    TetrisGraphics graphics(state);
    TetrisUI ui(state);

    // Render and game loop
    // --------------------
    while (!window.should_close()) {
        window.loop();
        input.loop();
        logic.loop();
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
