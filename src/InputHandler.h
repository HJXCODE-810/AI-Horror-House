#pragma once

#include <unordered_map>

// Forward declarations
class Game;
class Camera;

class InputHandler {
public:
    // Constructor that takes references to Game and Camera objects
    InputHandler(Game& game, Camera& camera);

    // Register GLUT callbacks
    void registerCallbacks();

    // Callback functions (must be static or global to be used by GLUT)
    static void keyboardCallback(unsigned char key, int x, int y);
    static void keyboardUpCallback(unsigned char key, int x, int y);
    static void specialKeysCallback(int key, int x, int y);
    static void mouseMoveCallback(int x, int y); // Passive motion
    static void mouseMotionCallback(int x, int y); // Active motion (button down)

    // Process held keys for smooth movement
    void processHeldKeys(float deltaTime);

    // Getters for key states
    bool isKeyPressed(unsigned char key) const;
    bool isSpecialKeyPressed(int key) const;

private:
    // Pointers to the game and camera instances to modify their state
    // Made static so static callbacks can access them
    static Game* s_gameInstance;
    static Camera* s_cameraInstance;
    static InputHandler* s_inputHandlerInstance; // Pointer to the instance for non-static methods

    // State tracking for keys
    // Using maps for flexibility, could use arrays for fixed keys
    std::unordered_map<unsigned char, bool> keyStates; // Tracks key presses/releases
    std::unordered_map<int, bool> specialKeyStates;    // Tracks special key presses/releases

    // Mouse state
    static int s_lastMouseX;    // Last recorded mouse X position
    static int s_lastMouseY;    // Last recorded mouse Y position
    static bool s_mouseWarped;  // Flag to ignore mouse jump after warping cursor

    // Non-static methods called by the static callbacks
    void handleKeyboard(unsigned char key, bool pressed);
    void handleSpecialKey(int key, bool pressed);
    void handleMouseMove(int x, int y);

    // Optional: Reset special key states to handle continuous press behavior
    void resetSpecialKeyStates(); 
};
