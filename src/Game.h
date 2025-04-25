#pragma once

#include "Config.h"
#include "Renderer.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "Camera.h"
#include "AudioManager.h"
#include "Maze.h"
#include "Ghost.h"
#include <memory> // For unique_ptr

// Main game class orchestrating all subsystems
class Game {
public:
    Game();
    ~Game();

    // Initialize all game systems and GLUT
    bool initialize(int argc, char** argv);

    // Start the main game loop
    void run();

    // Called periodically by GLUT timer
    void update(int value); // 'value' is unused here but required by glutTimerFunc

    // Called by GLUT for rendering
    void render();

    // Called by GLUT for window resizing
    void reshape(int width, int height);

    // --- Game Logic Actions ---
    void quitGame();
    void toggleLight();
    void interact(); // Player interaction (e.g., pick up key, open door)
    void flickerLight(int value); // Timer callback for light flickering
    void triggerGhostAppearance(int value); // Timer callback for ghost

    // --- Getters for Callbacks ---
    Camera& getCamera() { return camera; }
    Renderer& getRenderer() { return *renderer; } // Return reference

private:
    // Game state
    bool isRunning;
    bool gameWon;
    bool hasKey; // Does the player have the key?
    bool keyVisible;
    
    // Player start position
    float playerStartX, playerStartZ; // Initial position based on maze 'S'

    // Key position (example, could be placed dynamically)
    float keyX, keyZ;

    // Core systems (using unique_ptr for automatic memory management)
    TextureManager textureManager;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<InputHandler> inputHandler;
    Camera camera;
    AudioManager audioManager;
    Maze maze;
    Ghost ghost;

    // Timing
    int lastUpdateTime; // For calculating deltaTime

    // --- Static Wrappers for GLUT Callbacks ---
    // These functions call the corresponding methods on the singleton instance.
    static void displayCallback();
    static void reshapeCallback(int width, int height);
    static void timerCallback(int value); // For main update loop
    static void flickerTimerCallback(int value); // For light flicker
    static void ghostTimerCallback(int value); // For ghost appearance

    // Singleton instance pointer (required for static GLUT callbacks)
    static Game* instance;

    // --- Private Helper Methods ---
    void checkCollisions(); // Check player collision with walls, key, exit
    void setupTimers();     // Initialize GLUT timers
    void loadGameData();    // Load maze, place key, etc.
};
