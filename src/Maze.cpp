#include "Maze.h"
#include "Config.h"  // For MAZE_SIZE
#include <iostream>   // For debugging

// Constructor - initializes maze layout and start/end positions
Maze::Maze() {
    initializeLayout();
}

// Initialize the hardcoded maze layout
void Maze::initializeLayout() {
    // Example of a hardcoded maze layout; adjust as needed
    // Use 'W' for walls and ' ' for paths
    char maze[MAZE_SIZE][MAZE_SIZE] = {
        {'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},
        {'W', ' ', ' ', 'W', ' ', ' ', ' ', ' ', ' ', 'W'},
        {'W', ' ', 'W', 'W', 'W', 'W', 'W', 'W', ' ', 'W'},
        {'W', ' ', ' ', ' ', ' ', 'W', ' ', ' ', ' ', 'W'},
        {'W', ' ', 'W', 'W', ' ', 'W', ' ', 'W', ' ', 'W'},
        {'W', ' ', ' ', 'W', ' ', ' ', ' ', ' ', ' ', 'W'},
        {'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'}
    };

    // Copy maze layout into the class member
    for (int i = 0; i < MAZE_SIZE; ++i) {
        for (int j = 0; j < MAZE_SIZE; ++j) {
            layout[i][j] = maze[i][j];
        }
    }

    // Hardcode start and end positions (example)
    startRow = 1; startCol = 1;  // Starting position
    endRow = 5; endCol = 8;     // Ending position
}

// Get the character at a specific maze cell (row, col)
char Maze::getCell(int row, int col) const {
    if (row >= 0 && row < MAZE_SIZE && col >= 0 && col < MAZE_SIZE) {
        return layout[row][col];
    }
    return ' '; // Return empty space for out-of-bound access
}

// Check if a cell is a wall
bool Maze::isWall(int row, int col) const {
    if (row >= 0 && row < MAZE_SIZE && col >= 0 && col < MAZE_SIZE) {
        return layout[row][col] == 'W';  // Wall is represented by 'W'
    }
    return false; // Out of bounds is not considered a wall
}

// Get starting position (row, col)
void Maze::getStartPosition(int& startRowOut, int& startColOut) const {
    startRowOut = startRow;
    startColOut = startCol;
}

// Get ending position (row, col)
void Maze::getEndPosition(int& endRowOut, int& endColOut) const {
    endRowOut = endRow;
    endColOut = endCol;
}
