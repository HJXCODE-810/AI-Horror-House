void Camera::moveForward(float distance) {
    // Calculate movement direction based on yaw (angleY)
    float dx = sin(angleY) * distance;
    float dz = -cos(angleY) * distance; // Negative because Z decreases going forward in OpenGL
    x += dx;
    z += dz;
    // Optionally, check for collision or out-of-bounds in a game environment
}

void Camera::strafeRight(float distance) {
    // Calculate strafe direction (perpendicular to look direction)
    float dx = cos(angleY) * distance;
    float dz = sin(angleY) * distance;
    x += dx;
    z += dz;
}

void Camera::rotateY(float angle) {
    angleY += angle;
    angleY = fmod(angleY, 2.0f * M_PI); // Normalize to keep within 0 to 2π range
}

void Camera::rotateX(float angle) {
    angleX += angle;
    clampPitch();
}

void Camera::applyViewMatrix() const {
    // Set up the view matrix using gluLookAt or manual matrix calculation
    // Target point calculation:
    float lookX = x + sin(angleY) * cos(angleX);
    float lookY = y + sin(angleX);
    float lookZ = z - cos(angleY) * cos(angleX);

    // Up vector calculation (simplified, assumes no roll)
    float upX = 0.0f;
    float upY = 1.0f;
    float upZ = 0.0f;

    // Apply the lookAt transformation
    gluLookAt(x, y, z,       // Camera position (eye)
              lookX, lookY, lookZ, // Target point (center)
              upX, upY, upZ);      // Up vector
}
