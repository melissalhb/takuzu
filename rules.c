/**
 * FILENAME: rules.c
 *
 * AUTHORS: Audrey Damiba & Melissa Lacheb
 *
 * DESCRIPTION:
 *        Helper functions that check whether Takuzu grids are valid or completed.
 *
 * PUBLIC FUNCTIONS:
 *        int no_redundant_row_column(int **grid, int grid_size[2]);
 *        int is_valid_row_column(int *row_columnn, int row_column_size);
 *        int is_valid_grid(int **grid, int grid_size[2], int verbose);
 *        int is_solved(int **grid, int grid_size[2]);
 *
 **/

#include "rules.h"
#include "constants.h"
#include "utils.h"

#include <stdio.h>

/* Check if there are columns that are similar in a grid
Copied parameters : 
-int **grid : a 2D array which repesents a grid.
-int grid_size : contains the size of the grid in the X and Y dimension.
Return : int
*/
int no_redundant_row_column(int **grid, int grid_size[2]) {
  for (int i = 0; i < grid_size[0]; i++) {
    for (int j = 0; j < grid_size[1]; j++) {
      if (i != j) {
        if (is_same(grid[i], grid[j], grid_size[1])) {
          return -2;
        }

        if (is_same(get_column(grid, i, grid_size[0]),
                    get_column(grid, j, grid_size[1]), grid_size[0])) {
          return -1;
        }
      }
    }
  }
  return 1;
}

/* Checks if the rows and columns of the grid are valid and respect the rules of
the takuzu (if there are 3 consecutive zeros or ones) and returns different
numbers to display different error messages 
Copied parameters : *********
-int *row_column 
-int row_column_size 
Return : int* column
*/
int is_valid_row_column(int *row_columnn, int row_column_size) {
  int count[2] = {0, 0};
  int count_continuous[2] = {0, 0};

  for (int i = 0; i < row_column_size; i++) {
    if (row_columnn[i] != -1) {
      count[row_columnn[i]]++;
      if (i > 0) {
        if (row_columnn[i] == row_columnn[i - 1]) {
          count_continuous[row_columnn[i]]++;
        } else {
          count_continuous[row_columnn[i - 1]] = 0;
          count_continuous[row_columnn[i]] = 1;
        }
      } else {
        count_continuous[row_columnn[i]] = 1;
      }
    } else if (i > 0) {
      count_continuous[row_columnn[i - 1]] = 0;
    }

    if (count_continuous[0] >= 3) {
      return -4;
    }

    if (count_continuous[1] >= 3) {
      return -3;
    }

    if (count[0] > row_column_size / 2) {
      return -2;
    }

    if (count[1] > row_column_size / 2) {
      return -1;
    }
  }

  return 1;
}

/* Checks if the whole grid is valid and display errors messages if not
Copied parameters : 
-int *grid_size[2] :contains the size of the grid in the X and Y dimension
-int **grid : a 2D array which repesents a grid
-int verbose  ****
Return : int* column
*/
int is_valid_grid(int **grid, int grid_size[2], int verbose) {
  for (int i = 0; i < grid_size[0]; i++) {
    int *column = get_column(grid, i, grid_size[1]);
    int col_return = is_valid_row_column(column, grid_size[1]);
    int row_return = is_valid_row_column(grid[i], grid_size[0]);

    if (col_return != 1) {
      if (verbose) {
        switch (col_return) {
        case -4:
          printf(RED "Column %d has 3 continuous zeros.\n" RESET, i);
          break;
        case -3:
          printf(RED "Column %d has 3 continuous ones.\n" RESET, i);
          break;
        case -2:
          printf(RED "Column %d has too many zeros.\n" RESET, i);
          break;
        case -1:
          printf(RED "Column %d has too many ones.\n" RESET, i);
          break;
        }
      }
      return 0;
    }

    if (row_return != 1) {
      if (verbose) {
        switch (row_return) {
        case -4:
          printf(RED "Row %d has 3 continuous zeros.\n" RESET, i);
          break;
        case -3:
          printf(RED "Row %d has 3 continuous ones.\n" RESET, i);
          break;
        case -2:
          printf(RED "Row %d has too many zeros.\n" RESET, i);
          break;
        case -1:
          printf(RED "Row %d has too many ones.\n" RESET, i);
          break;
        }
      }
      return 0;
    }
  }

  int redundant_return = no_redundant_row_column(grid, grid_size);
  if (redundant_return != 1) {
    if (verbose) {
      switch (redundant_return) {
      case -2:
        printf(RED "The same row appears twice in the current grid.\n" RESET);
        break;
      case -1:
        printf(RED
               "The same column appears twice in the current grid.\n" RESET);
        break;
      }
    }
    return 0;
  }

  return 1;
}

/* Verify if a grid is full, verify its validity
Copied parameters : 
-int *grid_size[2] :contains the size of the grid in the X and Y dimension
-int **grid : a 2D array which repesents a grid
Return : int
*/
int is_solved(int **grid, int grid_size[2]) {
  for (int i = 0; i < grid_size[0]; i++) {
    for (int j = 0; j < grid_size[1]; j++) {
      if (grid[i][j] == -1) {
        return 0;
      }
    }
  }

  return is_valid_grid(grid, grid_size, 0);
}
