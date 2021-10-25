#include <stdlib.h>
#include <stdio.h>
#include "schelling.h"

struct Position {
    int x;
    int y;
};
typedef struct Position Position_t;

CellType** allocCells(size_t width, size_t height);
CellType** allocCells(size_t width, size_t height) {
    CellType** arr;

    // Allocate rows
    arr = malloc(sizeof(CellType*) * height);
    if (!arr) { return NULL; }
    for (int i = 0; i < height; i++) {
        // Allocate columns
        arr[i] = malloc(sizeof(CellType) * width);
        if (!arr[i]) { return NULL; }
    }

    return arr;
}

void schellingSwap(Schelling* schelling, int ax, int ay, int bx, int by);
void schellingSwap(Schelling* schelling, int ax, int ay, int bx, int by) {
    CellType tmp = schelling->grid[ay][ax];
    schelling->grid[ay][ax] = schelling->grid[by][bx];
    schelling->grid[by][bx] = tmp;
}

void posSwap(Position_t* arr, int a, int b);
void posSwap(Position_t* arr, int a, int b) {
    Position_t tmp = arr[a];
    arr[a] = arr[b];
    arr[b] = tmp;
}

Schelling *schellingInit(int height, int width, double probRed, double probBlue, double satisRatio) {
    // Check that there won't be too many reds or blues
    if (probRed + probBlue > 1.0) { return NULL; }

    // Allocate the structure in the heap
    Schelling* schel = malloc(sizeof(Schelling));
    if (!schel) { return NULL; }

    // Calculate the number of each type of element
    size_t gridSize = width * height;

    // Load parameter in the structure
    schel->width = width;
    schel->height = height;
    schel->satisRatio = satisRatio;

    // Allocate the raw array itself
    schel->grid = allocCells(width, height);
    if (!schel->grid) {
        free(schel);
        return NULL;
    }

    // Fill out cells
    schel->nbEmpty = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < height; x++) {
            // Use a random value between 0.0 and 1.0 to chose the cell type
            double linRand = (double)rand() / (double)RAND_MAX;
            if (linRand < probRed) {
                schel->grid[y][x] = RED;
            }
            else if (linRand < probRed + probBlue) {
                schel->grid[y][x] = BLUE;
            }
            else {
                schel->grid[y][x] = EMPTY;
                schel->nbEmpty++;
            }
        }
    }

    return schel;
}

int schellingIsUnsatisfied(Schelling* schelling, int h, int w) {
    int sameCount = 0;
    int counted = 0;

    // Get type of the cell to check
    CellType type = schelling->grid[h][w];

    // Count the number of identical cells directly around the cell
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            // Skip the cell itself
            if (dx == 0 && dy == 0) { continue; }

            // Calulate the position  of the cell to check
            int x = w + dx;
            int y = h + dy;

            // If it's out of bounds ignore it
            if (x < 0 || y < 0 || x >= schelling->width || y >= schelling->height) { continue; }

            // Count
            if (schelling->grid[y][x] != EMPTY) { counted++; }
            if (schelling->grid[y][x] == type) { sameCount++; }
        }
    }

    if (!counted) { return 1; }

    // Calculate the ratio and return accordingly
    double ratio = (double)sameCount / (double)counted;
    return (ratio < schelling->satisRatio);
}

// Complexity : O(n)
int schellingOneStep(Schelling *schelling) {
    // Allocate work arrays
    Position_t* emptyPos = malloc(sizeof(Position_t) * schelling->nbEmpty);
    Position_t* unsatisfiedPos = malloc(sizeof(Position_t) * schelling->width * schelling->height);
    
    // Save all empty and unsatisfied positions
    int emptyId = 0;
    int unsatisfiedCount = 0;
    for (int y = 0; y < schelling->height; y++) {
        for (int x = 0; x < schelling->width; x++) {
            // Save if empty cell
            if (schelling->grid[y][x] == EMPTY) {
                emptyPos[emptyId++] = (Position_t){.x = x, .y = y};
                continue;
            }

            // Save if unsatisfied cell
            if (schellingIsUnsatisfied(schelling, y, x)) {
                unsatisfiedPos[unsatisfiedCount++] = (Position_t){.x = x, .y = y};
            }
        }
    }

    // Relocate unsatisfied cells
    int unsatCount = unsatisfiedCount;
    while (unsatCount > 0) {
        // Get a random cell and swap it with a random empty cell
        int eId = (rand() % schelling->nbEmpty);
        int uId = (rand() % unsatCount);
        Position_t unsat = unsatisfiedPos[uId];
        Position_t empty = emptyPos[eId];
        schellingSwap(schelling, unsat.x, unsat.y, empty.x, empty.y);

        // Add the now empty cell back to the empty cell list
        emptyPos[eId] = unsat;

        // Remove the unsatified cell from the list
        unsatisfiedPos[uId] = unsatisfiedPos[--unsatCount];
    }

    // Free work arrays
    free(emptyPos);
    free(unsatisfiedPos);

    return unsatisfiedCount;
}

void schellingFree(Schelling *schelling) {
    if (!schelling) { return; }

    // Free each row
    for (int i = 0; i < schelling->height; i++) {
        free(schelling->grid[i]);
    }

    // Free the struct
    free(schelling);
}
