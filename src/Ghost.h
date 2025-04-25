#pragma once

#include "Config.h" // For constants (e.g., GHOST_SPEED, GHOST_VISIBLE_DURATION)
#include <cstdlib>   // For rand()
#include <ctime>     // For time()

class Camera; // Forward declaration to avoid circular dependency
class Maze;   // Forward declaration to avoid circular dependency

class Ghost {
public:
    // Constructor: Initializes the ghost with the maze for pathfinding and placement
    Ghost(const Maze& maze);

    // Update ghost state (movement, visibility timer)
    void update(float deltaTime, const Camera& playerCamera);

    // Getters for position, visibility, and angle
    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }
    float getAngle() const { return angle; } // Angle to face player
    bool isVisible() const { return visible; }

    // Trigger the ghost to appear at a random valid location in the maze
    void appearRandomly();

private:
    // Reference to maze data for pathfinding/placement
    const Maze& maze;

    // Ghost position (x, y, z), and angle (facing direction)
    float x, y, z;
    float angle;

    // Visibility state and timers for appearance logic
    bool visible;                // Is the ghost currently visible?
    float visibilityTimer;       // Time remaining for the ghost's visibility
    float timeUntilNextPossibleAppearance; // Timer until the next possible appearance

    // Movement-related properties
    float speed;                 // Speed of the ghost movement
    float targetX, targetZ;      // Target position for ghost movement

    // Private helper methods
    // Find a random non-wall location in the maze for spawning
    void findRandomSpawnPoint(float& outX, float& outZ);
};
