#include "Ghost.h"
#include "Camera.h" // Include Camera header
#include "Maze.h"   // Include Maze header
#include <cmath>    // For atan2, sqrt
#include <iostream> // For debugging

Ghost::Ghost(const Maze& mazeRef) :
    maze(mazeRef),
    x(0.0f), y(WALL_HEIGHT / 2.0f), z(0.0f), // Initial position (will be randomized)
    angle(0.0f),
    visible(false),
    visibilityTimer(0.0f),
    timeUntilNextPossibleAppearance(0.0f), // Appear immediately possibility
    speed(GHOST_SPEED),
    targetX(0.0f), targetZ(0.0f)
{
    // Seed random number generator
    srand(static_cast<unsigned int>(time(0)));

    // Set initial random position and target
    findRandomSpawnPoint(x, z);
    findRandomSpawnPoint(targetX, targetZ); // Initial random target
    y = WALL_HEIGHT / 2.0f; // Set Y position based on wall height
}

void Ghost::findRandomSpawnPoint(float& outX, float& outZ) {
    int r, c;
    do {
        r = rand() % maze.getHeight();
        c = rand() % maze.getWidth();
    } while (maze.isWall(r, c)); // Keep trying until a non-wall space is found

    // Convert maze coordinates (row, col) to world coordinates (x, z)
    // Assuming each maze cell is 1x1 unit in world space
    outX = static_cast<float>(c) + 0.5f; // Center of the cell
    outZ = static_cast<float>(r) + 0.5f; // Center of the cell
}

void Ghost::appearRandomly() {
    if (!visible && timeUntilNextPossibleAppearance <= 0.0f) {
        findRandomSpawnPoint(x, z);
        y = WALL_HEIGHT / 2.0f; // Reset height
        visible = true;
        visibilityTimer = static_cast<float>(GHOST_VISIBLE_DURATION) / 1000.0f; // Reset timer (in seconds)
        std::cout << "Ghost appeared at (" << x << ", " << z << ")" << std::endl; // Debug

        // Reset time for next *possible* appearance (random interval)
        timeUntilNextPossibleAppearance = static_cast<float>(rand() % (GHOST_APPEAR_INTERVAL_MAX - GHOST_APPEAR_INTERVAL_MIN + 1) + GHOST_APPEAR_INTERVAL_MIN) / 1000.0f;
    }
}

void Ghost::update(float deltaTime, const Camera& playerCamera) {
    if (visible) {
        visibilityTimer -= deltaTime;
        if (visibilityTimer <= 0.0f) {
            visible = false;
            std::cout << "Ghost disappeared." << std::endl; // Debug
            // Don't reset timeUntilNextPossibleAppearance here, it was set when it appeared
        } else {
            // --- Simple AI: Move towards target, face player ---

            // 1. Face the player
            float dx = playerCamera.getX() - x;
            float dz = playerCamera.getZ() - z;
            angle = atan2(dx, dz) * 180.0f / M_PI; // Calculate angle in degrees

            // 2. Move towards target (simple linear movement)
            // More complex pathfinding (A*) would be needed for maze navigation
            float moveDx = targetX - x;
            float moveDz = targetZ - z;
            float dist = sqrt(moveDx * moveDx + moveDz * moveDz);

            if (dist > 0.1f) { // If not already at target
                moveDx /= dist; // Normalize direction
                moveDz /= dist;
                x += moveDx * speed * deltaTime; // Apply speed and deltaTime
                z += moveDz * speed * deltaTime;

                // Basic collision detection with walls (crude)
                int currentR = static_cast<int>(z);
                int currentC = static_cast<int>(x);

                if (maze.isWall(currentR, currentC)) {
                    // Hit a wall, move back or find new target
                    x -= moveDx * speed * deltaTime; // Move back
                    z -= moveDz * speed * deltaTime;
                    findRandomSpawnPoint(targetX, targetZ); // Pick a new random target
                }
            } else {
                // Reached target, find a new random one
                findRandomSpawnPoint(targetX, targetZ);
            }
        }
    } else {
        // If not visible, decrease timer until next possible appearance
        if (timeUntilNextPossibleAppearance > 0.0f) {
            timeUntilNextPossibleAppearance -= deltaTime;
        }
        // Optionally, trigger appearance randomly here if timer <= 0
        // appearRandomly(); // Or trigger based on game events/AI
    }
}
