#pragma once

#include "Config.h" // For MAZE_SIZE
#include <vector>

class Maze {
public:
    // Constructor - Initializes the maze layout and start/end positions
    Maze();

    // Get the character at a specific maze cell (row, col)
    char getCell(int row, int col) const;

    // Check if a cell is a wall ('W') at (row, col)
    bool isWall(int row, int col) const;

    // Get starting position (row, col)
    void getStartPosition(int& startRow, int& startCol) const;

    // Get ending position (row, col)
    void getEndPosition(int& endRow, int& endCol) const;

    // Get maze dimensions (width and height are the same for a square maze)
    int getWidth() const { return MAZE_SIZE; }
    int getHeight() const { return MAZE_SIZE; }

    // Optional: Load maze from file or generate procedurally
    // bool loadFromFile(const std::string& filename);
    // void generateProcedural();

private:
    // Store the maze layout (2D array of characters)
    char layout[MAZE_SIZE][MAZE_SIZE];

    // Store start and end positions
    int startRow, startCol;
    int endRow, endCol;

    // Initialize the maze layout (hardcoded or procedural generation)
    void initializeLayout();
};
