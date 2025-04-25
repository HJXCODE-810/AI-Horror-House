// Renderer.h (Refactored)
#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <string>

// Forward declarations
class TextureManager;
class Camera;
class Maze;
class Ghost;

class Renderer {
public:
    explicit Renderer(TextureManager& texManager);
    ~Renderer();

    bool initialize();
    void reshape(int width, int height);
    void beginFrame(const Camera& camera);
    void endFrame();

    void drawRoom();
    void drawMaze(const Maze& maze);
    void drawFurniture(float x, float y, float z);
    void drawGhost(const Ghost& ghost);
    void drawDecorations();
    void drawUI(bool gameWon, bool hasKey);

    void setLight(bool on, float intensity);

private:
    TextureManager& textureManager;
    int windowWidth;
    int windowHeight;
    bool lightOn;
    float lightIntensity;
    bool fogEnabled;

    struct Bloodstain {
        float x, y, z;
        float size;
        float angle;
        int wallIndex;
    };

    std::vector<Bloodstain> bloodstains;

    void initializeBloodstains();
    void drawBloodstains();

    void drawWall(float x1, float y1, float z1,
                  float x2, float y2, float z2,
                  const std::string& textureName,
                  float texScaleX = 1.0f, float texScaleY = 1.0f);

    void drawFloor();
    void drawCeiling();
    void drawDoor(float x, float z, int orientation);
    void drawMannequin(float x, float y, float z, float rotation);
    void drawTable(float x, float y, float z);
    void drawChair(float x, float y, float z);
    void drawMirror(float x, float y, float z);

    void renderText(float x, float y, const std::string& text, void* font = GLUT_BITMAP_HELVETICA_18);

    void setupLighting();
    void setupFog();
};