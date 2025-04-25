#pragma once

#define _USE_MATH_DEFINES // For M_PI
#include <math.h>
#include "Config.h" // For constants

class Camera {
public:
    // Constructor
    Camera();

    // Set initial position
    void setPosition(float x, float y, float z);

    // Set initial orientation (angles in radians)
    void setOrientation(float angleX, float angleY);

    // Move the camera forward/backward along its look direction
    void moveForward(float distance);

    // Strafe the camera left/right perpendicular to its look direction
    void strafeRight(float distance);

    // Rotate the camera horizontally (yaw)
    void rotateY(float angle); // Angle in radians

    // Rotate the camera vertically (pitch)
    void rotateX(float angle); // Angle in radians

    // Apply the camera transformation (sets the view matrix)
    void applyViewMatrix() const;

    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }
    float getAngleX() const { return angleX; }
    float getAngleY() const { return angleY; }

    // Update camera based on mouse movement (delta from last position)
    void processMouseMovement(int deltaX, int deltaY, float sensitivity);

private:
    float x, y, z;        // Camera position
    float angleX, angleY; // Camera orientation (pitch, yaw) in radians

    // Clamp pitch to avoid flipping
    void clampPitch();
};
