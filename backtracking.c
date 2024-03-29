/**
* FILENAME: backtracking.c
*
* DESCRIPTION:
*       Functions that allows us to create a backtracking algorithm.
*
* PUBLIC FUNCTIONS:
*       int *find_next(int **grid, int grid_size[2]);
*       int solve(int **grid, int grid_size[2]);
*       int **generate_grid(int grid_size[2]);
*
* AUTHORS: Audrey Damiba & Melissa Lacheb
**/


#include "backtracking.h"
#include "utils.h"
#include "rules.h"

#include <stdlib.h>

/* Finds in a grid the position of the first empty cell encountered 
Copied parameters : 
-int **grid : a 2D array represents a grid
_int grid_size[2] : contains the size of the grid in the X and Y dimension
Return :
int* next
*/
int *find_next(int **grid, int grid_size[2]) {
  int *next = malloc(sizeof(int) * 2);
  next[0] = -1;
  next[1] = -1;

  for (int i = 0; i < grid_size[0]; i++) {
    for (int j = 0; j < grid_size[1]; j++) {
      if (grid[i][j] == -1) {
        next[0] = i;
        next[1] = j;
        return next;
      }
    }
  }

  return next;
}

/* Solve the grid automatically according to the rules
Copied parameters : 
-int **grid : a 2D array represents a grid
_int grid_size[2] : contains the size of the grid in the X and Y dimension
Return :
int
*/
int solve(int **grid, int grid_size[2]) {
  int *next = find_next(grid, grid_size);
  if (next[0] == -1) {
    return 1;
  }

  int val = rand() % 2;
  for (int i = 0; i < 2; i++) {
    grid[next[0]][next[1]] = val;

    if (is_valid_grid(grid, grid_size, 0)) {
      if (solve(grid, grid_size)) {
        return 1;
      }
    }

    grid[next[0]][next[1]] = -1;
    val = 1 - val;
  }

  return 0;
}

/* Create and generate a solved grid
Copied parameters : 
_int grid_size[2] : contains the size of the grid in the X and Y dimension
Return :
int** grid
*/
int **generate_grid(int grid_size[2]) {
  int **grid = create_grid(grid_size, -1);
  solve(grid, grid_size);

  return grid;
}

