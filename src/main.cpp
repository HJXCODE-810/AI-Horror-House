#include "Game.h"
#include <iostream>
#include <memory>     // For std::unique_ptr
#include <stdexcept>  // For exception handling

int main(int argc, char** argv) {
    // Create the game instance using smart pointer for automatic memory management
    std::unique_ptr<Game> game = std::make_unique<Game>();

    try {
        // Initialize the game
        if (!game->initialize(argc, argv)) {
            std::cerr << "[Error] Game initialization failed. Exiting.\n";
            return 1;
        }

        // Run the game loop
        game->run();

    } catch (const std::exception& e) {
        std::cerr << "[Exception] " << e.what() << std::endl;
        return 2;
    } catch (...) {
        std::cerr << "[Error] Unknown exception occurred.\n";
        return 3;
    }

    std::cout << "Game exited successfully.\n";
    return 0;
}
