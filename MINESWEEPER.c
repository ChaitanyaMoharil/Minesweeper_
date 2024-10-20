#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 9 // Size of the grid (9x9)
#define MINES 10 // Number of mines

// Grid values
#define MINE -1
#define COVERED 0
#define UNCOVERED 1
#define FLAGGED 2

typedef struct {
    int value; // -1 for mine, number of adjacent mines otherwise
    int state; // 0 = covered, 1 = uncovered, 2 = flagged
} Cell;

Cell grid[SIZE][SIZE];

// Function prototypes
void initializeGrid();
void placeMines();
void calculateNumbers();
void printGrid(int showMines);
int isValid(int x, int y);
void uncover(int x, int y);
void floodFill(int x, int y);
int checkWin();
void playGame();

int main() {
    srand(time(NULL)); // Seed for random number generation
    initializeGrid();
    playGame();
    return 0;
}

// Initialize the grid: Set all cells to covered, no mines
void initializeGrid() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            grid[i][j].value = 0; // No mines initially
            grid[i][j].state = COVERED; // All cells are covered
        }
    }
    placeMines();
    calculateNumbers();
}

// Randomly place mines on the grid
void placeMines() {
    int placedMines = 0;
    while (placedMines < MINES) {
        int x = rand() % SIZE;
        int y = rand() % SIZE;
        if (grid[x][y].value != MINE) {
            grid[x][y].value = MINE;
            placedMines++;
        }
    }
}

// Calculate the number of adjacent mines for each cell
void calculateNumbers() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j].value == MINE) continue;
            int mineCount = 0;
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = i + dx;
                    int ny = j + dy;
                    if (isValid(nx, ny) && grid[nx][ny].value == MINE) {
                        mineCount++;
                    }
                }
            }
            grid[i][j].value = mineCount;
        }
    }
}

// Check if coordinates are valid within the grid
int isValid(int x, int y) {
    return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
}

// Print the grid (optionally showing mines for debug or game over)
void printGrid(int showMines) {
    printf("   ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d  ", i);
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j].state == COVERED) {
                printf("# ");
            } else if (grid[i][j].state == FLAGGED) {
                printf("F ");
            } else if (grid[i][j].value == MINE && showMines) {
                printf("* ");
            } else {
                printf("%d ", grid[i][j].value);
            }
        }
        printf("\n");
    }
}

// Uncover a cell, handle game over if it's a mine
void uncover(int x, int y) {
    if (!isValid(x, y) || grid[x][y].state != COVERED) return;
    if (grid[x][y].value == MINE) {
        printf("Game Over! You hit a mine.\n");
        printGrid(1); // Show all mines
        exit(0);
    }
    grid[x][y].state = UNCOVERED;
    if (grid[x][y].value == 0) {
        floodFill(x, y);
    }
}

// Recursive flood fill for uncovering blank areas
void floodFill(int x, int y) {
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int nx = x + dx;
            int ny = y + dy;
            if (isValid(nx, ny) && grid[nx][ny].state == COVERED) {
                uncover(nx, ny);
            }
        }
    }
}

// Check if all non-mine cells are uncovered
int checkWin() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j].value != MINE && grid[i][j].state == COVERED) {
                return 0; // Still cells to uncover
            }
        }
    }
    return 1; // All non-mine cells uncovered, player wins
}

// Main game loop
void playGame() {
    int x, y;
    while (!checkWin()) {
        printGrid(0); // Show the grid without revealing mines
        printf("Enter coordinates to uncover (x y): ");
        scanf("%d %d", &x, &y);
        uncover(x, y);
    }
    printf("Congratulations! You win.\n");
    printGrid(1); // Show all mines after winning
}