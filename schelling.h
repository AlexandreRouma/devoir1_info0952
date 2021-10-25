#ifndef SCHELLING_H
#define SCHELLING_H

typedef enum {
    EMPTY,
    RED,
    BLUE
} CellType;

typedef struct {
    int height, width;
    CellType **grid;
    int nbEmpty;
    double satisRatio;
} Schelling;

/* -------------------------------------------------------------------- *
 * Allocate the memory for a schelling model of a given height and width,
 * and set the height, width and satisRatio attributes according to the
 * function arguments.
 *
 * Initialise the grid randomly according to the probRed and probBlue 
 * probabilities (see the project statement for details). probRed+probBlue
 * are supposed to belong to [0.0,1.0] and such that probRed+probBlue<=1.0.
 *
 * Return NULL if memory allocation was not possible.
 *
 * ARGUMENTS
 * height        The height of the grid
 * width         The width of the grid
 * probRed       The probability that a cell is initially RED
 * probBlue      The probability that a cell is initially BLUE
 * satisRatio    The satisfaction ratio associated to the model
 *
 * RETURN
 * schelling     A pointer to the created Schelling model or NULL
 *
 * NOTE
 * The function should not modify the random seed. The returned model is 
 * supposed to be cleaned with schellingFree after usage.
 * --------------------------------------------------------------------*/

Schelling *schellingInit(int height, int width, double probRed, double probBlue, double satisRatio);

/* -------------------------------------------------------------------- *
 * Free the memory taken by the schelling model, including the grid.
 *
 * ARGUMENT
 * schelling  The schelling model to be freed
 *
 * NOTE
 * The returned matrix should be cleaned with matrix_free after usage
 * --------------------------------------------------------------------*/

void schellingFree(Schelling *schelling);

/* -------------------------------------------------------------------- *
 * Determine whether the student at position (h, w) in the grid is 
 * unsatisfied.
 *
 * ARGUMENTS
 * schelling  a Schelling model
 * h          the height of the position
 * w          the width of the position
 *
 * RETURN
 * satis     -1 if position (h,w) is empty or outside the grid, 0 if 
 *           the student at (h,w) is satisfied, 1 if the student is
 *           unsatisfied.       
 *
 * --------------------------------------------------------------------*/

int schellingIsUnsatisfied(Schelling *schelling, int h, int w);

/* -------------------------------------------------------------------- *
 * Execute one step of the simulation of the Schelling model in argument
 * (see the project statement for the details). The procedure should
 * update the model taken as argument
 *
 * ARGUMENT
 * Schelling  a Schelling model
 *
 * RETURN
 * nb         The number of students who have moved during the step
 *
 * --------------------------------------------------------------------*/

int schellingOneStep(Schelling *schelling);

#endif
