#include "Game.h"
#include <GL/glew.h> // Must be included before freeglut
#include <GL/freeglut.h>
#include <iostream>
#include <cmath> // For distance calculation

// Initialize static instance pointer
Game* Game::instance = nullptr;

Game::Game() :
    isRunning(false),
    gameWon(false),
    hasKey(false),
    playerStartX(1.5f), // Default, will be updated from maze
    playerStartZ(1.5f),
    keyX(0.0f), keyZ(0.0f), // Will be placed during load
    keyVisible(true),
    lastUpdateTime(0),
    maze(), // Initialize maze
    ghost(maze) // Initialize ghost, passing maze reference
    // textureManager is default constructed
    // camera is default constructed
    // audioManager is default constructed
{
    if (instance == nullptr) {
        instance = this;
    } else {
        // Handle error: Trying to create multiple Game instances
        std::cerr << "Error: Multiple Game instances requested." << std::endl;
        // Consider throwing an exception or exiting
    }
}

Game::~Game() {
    audioManager.shutdown();
    if (instance == this) {
        instance = nullptr;
    }
}

bool Game::initialize(int argc, char** argv) {
    std::cout << "Initializing Game..." << std::endl;

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("AI Haunted House");

    // Initialize Renderer (which initializes GLEW and Textures)
    renderer = std::make_unique<Renderer>(textureManager);
    if (!renderer->initialize()) {
        std::cerr << "Renderer initialization failed." << std::endl;
        return false;
    }

    // Initialize Input Handler
    inputHandler = std::make_unique<InputHandler>(*this, camera);
    inputHandler->registerCallbacks(); // Register GLUT input callbacks AFTER window creation

    // Initialize Audio Manager
    if (!audioManager.initialize()) {
        std::cerr << "Audio Manager initialization failed." << std::endl;
        // Continue without audio? Or return false?
    }

    // Load game data (maze, player start, key position)
    loadGameData();

    // Set initial camera position based on maze start
    camera.setPosition(playerStartX, PLAYER_EYE_HEIGHT, playerStartZ);

    // Register other GLUT callbacks using static wrappers
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);

    // Get initial time for update loop
    lastUpdateTime = glutGet(GLUT_ELAPSED_TIME);

    // Setup timers for update loop, flickering, ghost
    setupTimers();

    // Start background ambient sound
    audioManager.loadSound(SOUND_AMBIENT); // Ensure it's loaded
    audioManager.playAmbientSound(SOUND_AMBIENT, true);

    isRunning = true;
    std::cout << "Game Initialization Complete." << std::endl;
    return true;
}

void Game::loadGameData() {
    // Get player start position from maze
    int startR, startC;
    maze.getStartPosition(startR, startC);
    playerStartX = static_cast<float>(startC) + 0.5f;
    playerStartZ = static_cast<float>(startR) + 0.5f;

    // Place the key at a specific location (example: near the middle)
    // Could be randomized or read from level data
    int keyR = MAZE_SIZE / 2;
    int keyC = MAZE_SIZE / 3;
    // Ensure key is not placed inside a wall
    while (maze.isWall(keyR, keyC)) {
        keyR = rand() % MAZE_SIZE;
        keyC = rand() % MAZE_SIZE;
    }
    keyX = static_cast<float>(keyC) + 0.5f;
    keyZ = static_cast<float>(keyR) + 0.5f;
    keyVisible = true;
    hasKey = false; // Reset key status
    gameWon = false; // Reset win status

    std::cout << "Player Start: (" << playerStartX << ", " << playerStartZ << ")" << std::endl;
    std::cout << "Key Position: (" << keyX << ", " << keyZ << ")" << std::endl;
}

void Game::run() {
    if (!isRunning) {
        std::cerr << "Game not initialized. Call initialize() first." << std::endl;
        return;
    }
    std::cout << "Starting Game Loop..." << std::endl;
    glutMainLoop(); // Start the GLUT event processing loop
}

void Game::setupTimers() {
    // Main update loop timer (e.g., aiming for ~60 FPS)
    int updateInterval = 1000 / 60; // milliseconds
    glutTimerFunc(updateInterval, timerCallback, 0);

    // Light flicker timer
    glutTimerFunc(FLICKER_INTERVAL, flickerTimerCallback, 0);

    // Initial ghost appearance timer (random delay)
    int ghostDelay = rand() % (GHOST_APPEAR_INTERVAL_MAX - GHOST_APPEAR_INTERVAL_MIN + 1) + GHOST_APPEAR_INTERVAL_MIN;
    glutTimerFunc(ghostDelay, ghostTimerCallback, 0);
}

// --- GLUT Callback Wrappers ---

void Game::displayCallback() {
    if (instance) {
        instance->render();
    }
}

void Game::reshapeCallback(int width, int height) {
    if (instance) {
        instance->reshape(width, height);
    }
}

void Game::timerCallback(int value) {
    if (instance && instance->isRunning) {
        instance->update(value);
        // Re-register the timer for the next update cycle
        int updateInterval = 1000 / 60;
        glutTimerFunc(updateInterval, timerCallback, 0); // Pass 0 as value
    }
}

void Game::flickerTimerCallback(int value) {
    if (instance && instance->isRunning) {
        instance->flickerLight(value);
        // Re-register the timer
        glutTimerFunc(FLICKER_INTERVAL, flickerTimerCallback, 0);
    }
}

void Game::ghostTimerCallback(int value) {
    if (instance && instance->isRunning) {
        instance->triggerGhostAppearance(value);
        // Re-register the timer for the *next* possible appearance check
        int nextCheckDelay = rand() % (GHOST_APPEAR_INTERVAL_MAX - GHOST_APPEAR_INTERVAL_MIN + 1) + GHOST_APPEAR_INTERVAL_MIN;
        glutTimerFunc(nextCheckDelay, ghostTimerCallback, 0);
    }
}

// --- Update and Render ---

void Game::update(int value) {
    if (gameWon) return; // Stop updates if game is won

    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = static_cast<float>(currentTime - lastUpdateTime) / 1000.0f; // Delta time in seconds
    lastUpdateTime = currentTime;

    // Process held keys for movement
    inputHandler->processHeldKeys(deltaTime);

    // Store camera position before collision detection
    float oldCamX = camera.getX();
    float oldCamZ = camera.getZ();

    // Update ghost logic
    ghost.update(deltaTime, camera);

    // Check collisions (player vs maze, key, exit)
    checkCollisions();

    // If collision occurred, revert camera position (simple wall collision)
    // More sophisticated collision response (sliding) would be better.
    int currentR = static_cast<int>(camera.getZ());
    int currentC = static_cast<int>(camera.getX());
    if (maze.isWall(currentR, currentC)) {
        // Check which direction caused collision and revert only that axis?
        // Simpler: revert both for now
        camera.setPosition(oldCamX, camera.getY(), oldCamZ);
        // Optionally play a bump sound
        // audioManager.playSound("sounds/bump.wav");
    }

    // Force redraw
    glutPostRedisplay();
}

void Game::render() {
    if (!renderer) return;

    renderer->beginFrame(camera);

    // Draw scene elements
    renderer->drawMaze(maze);
    renderer->drawFurniture();
    renderer->drawDecorations();

    // Draw the key if it's visible
    if (keyVisible) {
        // Draw a simple representation for the key (e.g., a small yellow cube/sphere)
        glPushMatrix();
        glTranslatef(keyX, 0.3f, keyZ); // Position slightly above floor
        glDisable(GL_TEXTURE_2D); // Use color for key
        glDisable(GL_LIGHTING);   // Make key bright
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow
        glutSolidCube(0.15f); // Small cube
        glColor3f(1.0f, 1.0f, 1.0f); // Reset color
        if (renderer->setLight) glEnable(GL_LIGHTING); // Re-enable lighting if it was on
        glEnable(GL_TEXTURE_2D);
        glPopMatrix();
    }

    // Draw the ghost
    renderer->drawGhost(ghost);

    // Draw UI elements (on top)
    renderer->drawHUD();

    // End frame (swap buffers)
    glutSwapBuffers();
}

void Game::flickerLight(int value) {
    // Randomly flicker light intensity or turn it off completely for short periods
    // E.g., dim lights
    renderer->flickerLights();
}

void Game::triggerGhostAppearance(int value) {
    // Make ghost appear at a random location in the maze
    // If the ghost is not already visible, spawn it
    ghost.spawnRandom();
}
