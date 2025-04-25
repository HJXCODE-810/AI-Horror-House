#include "InputHandler.h"
#include "Game.h"
#include "Camera.h"
#include "Config.h"
#include <GL/glew.h> // Must be included before freeglut
#include <GL/freeglut.h>
#include <iostream> // For debugging

// Initialize static members
Game* InputHandler::s_gameInstance = nullptr;
Camera* InputHandler::s_cameraInstance = nullptr;
InputHandler* InputHandler::s_inputHandlerInstance = nullptr;
int InputHandler::s_lastMouseX = WINDOW_WIDTH / 2;
int InputHandler::s_lastMouseY = WINDOW_HEIGHT / 2;
bool InputHandler::s_mouseWarped = false;


InputHandler::InputHandler(Game& game, Camera& camera) {
    // Set static pointers to allow callbacks access
    s_gameInstance = &game;
    s_cameraInstance = &camera;
    s_inputHandlerInstance = this; // Store instance pointer

    // Initialize mouse position
    glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    s_mouseWarped = true; // Ignore the first motion event after warp
}

void InputHandler::registerCallbacks() {
    glutKeyboardFunc(keyboardCallback);
    glutKeyboardUpFunc(keyboardUpCallback);
    glutSpecialFunc(specialKeysCallback);
    glutMotionFunc(mouseMotionCallback); // Called when mouse moves WHILE a button is pressed
    glutPassiveMotionFunc(mouseMotionCallback); // Called when mouse moves WITHOUT button press - needed for FPS look

    // Hide cursor and keep it centered
    glutSetCursor(GLUT_CURSOR_NONE);
}

// --- Static Callback Implementations ---

void InputHandler::keyboardCallback(unsigned char key, int x, int y) {
    if (s_inputHandlerInstance) {
        s_inputHandlerInstance->handleKeyboard(key, true); // Key pressed
    }
    // Handle immediate actions like toggles or exit
    if (key == 27) { // ESC key
        if (s_gameInstance) s_gameInstance->quitGame();
    }
    if (key == 'l' || key == 'L') { // Toggle light
        if (s_gameInstance) s_gameInstance->toggleLight();
    }
    // Add other immediate key actions here (e.g., interaction 'e')
    if (key == 'e' || key == 'E') {
        if (s_gameInstance) s_gameInstance->interact();
    }
}

void InputHandler::keyboardUpCallback(unsigned char key, int x, int y) {
    if (s_inputHandlerInstance) {
        s_inputHandlerInstance->handleKeyboard(key, false); // Key released
    }
}

void InputHandler::specialKeysCallback(int key, int x, int y) {
    if (s_inputHandlerInstance) {
        s_inputHandlerInstance->handleSpecialKey(key, true); // Special key pressed
    }
}

void InputHandler::mouseMoveCallback(int x, int y) {
    // Not used in this setup, using mouseMotionCallback instead
}

void InputHandler::mouseMotionCallback(int x, int y) {
    if (s_inputHandlerInstance) {
        s_inputHandlerInstance->handleMouseMove(x, y);
    }
}

// --- Non-Static Member Functions ---

void InputHandler::handleKeyboard(unsigned char key, bool pressed) {
    // Normalize to lowercase for consistent checks
    unsigned char lowerKey = tolower(key);
    keyStates[lowerKey] = pressed;
    // std::cout << "Key: " << lowerKey << " Pressed: " << pressed << std::endl; // Debug
}

void InputHandler::handleSpecialKey(int key, bool pressed) {
    specialKeyStates[key] = pressed;
    // std::cout << "Special Key: " << key << " Pressed: " << pressed << std::endl; // Debug
}

void InputHandler::handleMouseMove(int x, int y) {
    // If the mouse was just warped, ignore this event to prevent sudden jump
    if (s_mouseWarped) {
        s_mouseWarped = false;
        s_lastMouseX = x;
        s_lastMouseY = y;
        return;
    }

    int deltaX = x - s_lastMouseX;
    int deltaY = y - s_lastMouseY; // Might need inversion depending on coordinate system

    s_lastMouseX = x;
    s_lastMouseY = y;

    if (s_cameraInstance) {
        s_cameraInstance->processMouseMovement(deltaX, deltaY, PLAYER_ROTATE_SPEED);
    }

    // Prevent cursor from leaving the window by warping it back to the center
    // Check if cursor is near the edge (optional, can just warp every frame)
    if (x < 100 || x > WINDOW_WIDTH - 100 || y < 100 || y > WINDOW_HEIGHT - 100) {
        glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        s_lastMouseX = WINDOW_WIDTH / 2; // Reset last position after warp
        s_lastMouseY = WINDOW_HEIGHT / 2;
        s_mouseWarped = true; // Set flag to ignore next motion event
    }
}

void InputHandler::processHeldKeys(float deltaTime) {
    if (!s_cameraInstance) return;

    float moveDistance = PLAYER_MOVE_SPEED * deltaTime; // Adjust based on deltaTime if needed

    // Forward/Backward Movement (W/S)
    if (isKeyPressed('w')) {
        s_cameraInstance->moveForward(moveDistance);
    }
    if (isKeyPressed('s')) {
        s_cameraInstance->moveForward(-moveDistance);
    }

    // Strafe Left/Right (A/D)
    if (isKeyPressed('a')) {
        s_cameraInstance->strafeRight(-moveDistance);
    }
    if (isKeyPressed('d')) {
        s_cameraInstance->strafeRight(moveDistance);
    }

    // Handle special keys for movement if needed (e.g., arrow keys)
    if (isSpecialKeyPressed(GLUT_KEY_UP)) {
        s_cameraInstance->moveForward(moveDistance);
    }
    if (isSpecialKeyPressed(GLUT_KEY_DOWN)) {
        s_cameraInstance->moveForward(-moveDistance);
    }
    if (isSpecialKeyPressed(GLUT_KEY_LEFT)) {
        s_cameraInstance->strafeRight(-moveDistance);
    }
    if (isSpecialKeyPressed(GLUT_KEY_RIGHT)) {
        s_cameraInstance->strafeRight(moveDistance);
    }
}

bool InputHandler::isKeyPressed(unsigned char key) const {
    unsigned char lowerKey = tolower(key);
    auto it = keyStates.find(lowerKey);
    return (it != keyStates.end() && it->second);
}

bool InputHandler::isSpecialKeyPressed(int key) const {
    auto it = specialKeyStates.find(key);
    return (it != specialKeyStates.end() && it->second);
}
