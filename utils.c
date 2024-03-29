/**
 * FILENAME: utils.c
 *
 * AUTHORS: Audrey Damiba & Melissa Lacheb
 *
 * DESCRIPTION:
 *        Helper functions that help deal with dynamic arrays, masks and grids.
 *
 * PUBLIC FUNCTIONS:
 *        int **create_grid(int grid_size[2], int initial_value)
 *        void print_grid(int **grid, int grid_size[2])
 *        int **generate_mask(int grid_size[2])
 *        int **get_grid_from_mask(int **mask,
 *                                int **original_grid,
 *                                int grid_size[2])
 *
 **/

#include "utils.h"
#include "constants.h"

#include <stdio.h>
#include <stdlib.h>

/** Creates a grid with size grid_size.

Copied parameters: 
 - int grid_size[2]: contains the size of the grid in the X and Y dimension
 - int initial_value: the initial value of each cell of the grid

Returns: 
 - int**: the resulting grid
**/
int **create_grid(int grid_size[2], int initial_value) {
  int **grid = (int **)malloc(grid_size[0] * sizeof(int *));

  for (int i = 0; i < grid_size[0]; i++) {
    grid[i] = (int *)malloc(grid_size[1] * sizeof(int));
    for (int j = 0; j < grid_size[1]; j++) {
      grid[i][j] = initial_value;
    }
  }
  return grid;
}

/** Pretty-prints game and mask grids.

Copied parameters:
 - int **grid: 2D grid to pretty-print
 - int grid_size[2]: contains the size of the grid in the X and Y dimension

No return
**/
void print_grid(int **grid, int grid_size[2]) {
  printf("\n      ");
  for (int i = 65; i < 65 + grid_size[1]; i++) {
    printf("  %c   ", i);
  }
  printf("\n     ╔═════");
  for (int i = 1; i < grid_size[1] - 1; i++) {
    printf("╦═════");
  }
  printf("╦═════╗\n");

  for (int i = 0; i < grid_size[0]; i++) {
    printf("%02d   ║", i + 1);
    for (int j = 0; j < grid_size[1]; j++) {
      if (grid[i][j] == -1) {
        printf("  *  ║");
      } else if (grid[i][j] == 1) {
        printf(YELLOW "  %d  " RESET "║", grid[i][j]);
      } else if (grid[i][j] == 0) {
        printf(GREEN "  %d  " RESET "║", grid[i][j]);
      } else if (grid[i][j] == VALID_MASK) {
        printf(GREEN " ■■■ " RESET "║");
      } else {
        printf("     ║");
      }
    }

    if (i < grid_size[1] - 1) {
      printf("\n     ║");
      for (int j = 0; j < grid_size[1]; j++) {
        printf("═════");
        if (j == grid_size[1] - 1) {
          printf("║");
        } else {
          printf("╬");
        }
      }
    }
    printf("\n");
  }

  printf("     ╚═════");
  for (int i = 1; i < grid_size[1] - 1; i++) {
    printf("╩═════");
  }
  printf("╩═════╝\n");
}

/** Generates a random mask.

Copied parameter: 
 - int grid_size[2]: contains the size of the grid in the X and Y dimension

Returns: 
 - int**: the resulting mask
**/
int **generate_mask(int grid_size[2]) {
  int **mask = create_grid(grid_size, INVALID_MASK);

  for (int i = 0; i < grid_size[0]; i++) {
    for (int j = 0; j < grid_size[1]; j++) {
      mask[i][j] = (rand() % 2) ? VALID_MASK : INVALID_MASK;
    }
  }
  return mask;
}

/** Asks the user to generate a grid.

Copied parameter:
 - int grid_size[2]: contains the size of the grid in the X and Y dimension
 
Returns: 
 - int**: the resulting mask
**/
int **generate_mask_from_user(int grid_size[2]) {
  int **mask = create_grid(grid_size, INVALID_MASK);

  for (int i = 0; i < grid_size[0]; i++) {
    for (int j = 0; j < grid_size[1]; j++) {
      mask[i][j] = -1;
      printf(BLU "\nCurrent mask\n" RESET);
      print_grid(mask, grid_size);

      int choice;
      printf(CYN "\nDo you want to hide the game grid at (%02d,%c) during the "
                 "game? (0 = hide, 1 = show)\n\n" RESET,
             i + 1, j + 65);
      do {
        scanf("%d", &choice);
        if (choice != 1 && choice != 0) {
          printf(RED "Invalid choice: choose 1 or 0.\n");
        }
      } while (choice != 1 && choice != 0);
      mask[i][j] = choice ? VALID_MASK : INVALID_MASK;
    }
  }

  printf(GREEN "\nHere is your generated mask!\n" RESET);
  print_grid(mask, grid_size);

  return mask;
}

/** Get the grid with the mask applied on it.

Copied parameters:
 - int **mask: 2D array which contains the mask
 - int **original_grid: 2D array which contains a solution grid
 - int grid_size[2]: contains the size of the grid in the X and Y dimension

Returns:
 - int**: the correct grid without the masked values
**/
int **get_grid_from_mask(int **mask, int **original_grid, int grid_size[2]) {
  int **grid = create_grid(grid_size, -1);
  for (int i = 0; i < grid_size[0]; i++) {
    for (int j = 0; j < grid_size[1]; j++) {
      if (mask[i][j] == VALID_MASK) {
        grid[i][j] = original_grid[i][j];
      }
    }
  }

  return grid;
}

/** Verify if two arrays are the same, returns 1 if it is, returns 0 if it's not

Copied parameters :
 - int *arr1, int *arr2: the two arrays to compare
 - int arr_size: the size of two arrays

Returns:
 - int: wheter the two arrays are the same
**/
int is_same(int *arr1, int *arr2, int arr_size) {
  for (int i = 0; i < arr_size; i++) {
    if (arr1[i] == -1 || arr2[i] == -1 || arr1[i] != arr2[i]) {
      return 0;
    }
  }
  return 1;
}

/* Take a colum in the grid and put it in a list

Copied parameters : 
  in-t **grid, a 2D array which represents the grid
int col_idx, index in the colunm in the grid
int col_size, the size of an array column
Return : int* column
*/
int *get_column(int **grid, int col_idx, int col_size) {
  int *column = malloc(sizeof(int) * col_size);
  for (int i = 0; i < col_size; i++) {
    column[i] = grid[i][col_idx];
  }
  return column;
}