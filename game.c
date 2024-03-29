/**
 * FILENAME: game.c
 *
 * DESCRIPTION:
 *        Menu and game-related utilities.
 *
 * PUBLIC FUNCTIONS:
 *        void autogame(int grid_size[2])
 *        void menu()
 *        void get_move(int *i, int *j, int *move, int grid_size[2]);
 *        void game(int grid_size[2]);
 *
 * AUTHORS: Audrey Damiba & Melissa Lacheb
 **/

#include "game.h"
#include "backtracking.h"
#include "constants.h"
#include "rules.h"
#include "utils.h"

#include <stdio.h>

/*Gets the move the player wants to do when they play
Copied parameters :
-int *i, int *j :
-int *move :
-int grid_size[2] : contains the size of the grid in the X and Y dimension
*/
void get_move(int *i, int *j, int *move, int grid_size[2]) {
  do {
    char col_name;
    printf("Where do you want to move? (row,column)\n");
    scanf("%d,%c", i, &col_name);

    *j = ((int)col_name) - 65;
    *i -= 1;

    if (*j < 0 || *i < 0 || *i >= grid_size[0] || *j >= grid_size[1]) {
      printf(RED "Invalid.\n" RESET);
    }
  } while (*j < 0 || *i < 0 || *i >= grid_size[0] || *j >= grid_size[1]);

  do {
    printf("\nWhat move do you want to make? (0,1)\n");
    scanf("%d", move);
    if (*move != 0 && *move != 1) {
      printf(RED "Invalid.\n" RESET);
    }
  } while (*move != 0 && *move != 1);

  printf("\n");
}

/*Runs the game
Copied parameter :
-int grid_size[2] : contains the size of the grid in the X and Y dimension
*/
void game(int grid_size[2]) {
  int lives = 3;
  int clues = 3;

  int **correct_grid = generate_grid(grid_size);
  int **mask = generate_mask(grid_size);

  int choice;
  do {
    printf(CYN "\n\n.・。.・゜✭・.  " BOLD UNDERLINE "Action" RESET CYN
               "  .・。.・゜✭・.\n\n"
               "What do you want to do?\n\n"
               " - Type 1 to generate a mask manually\n"
               " - Type 2 to generate a mask automatically\n"
               " - Type 3 to test the current mask\n"
               " - Type 4 to generate a new base grid\n"
               " - Type 5 to display the base grid (" BOLD UNDERLINE
               "CHEAT" RESET CYN ")\n"
               " - Type 6 to play\n\n"
               ".・。.・゜✭・.  " BOLD UNDERLINE "Action" RESET CYN
               "  .・。.・゜✭・.\n\n" RESET);

    scanf("%d", &choice);
    if (choice < 1 || choice > 6) {
      printf(RED "Invalid choice." RESET);
    } else if (choice == 1) {
      mask = generate_mask_from_user(grid_size);
    } else if (choice == 2) {
      mask = generate_mask(grid_size);
      printf(BLU "\nNew mask\n" RESET);
      print_grid(mask, grid_size);
    } else if (choice == 3) {
      printf(BLU "\nMask\n" RESET);
      print_grid(mask, grid_size);
      printf(BLU "\n\nGrid after mask\n" RESET);
      print_grid(get_grid_from_mask(mask, correct_grid, grid_size), grid_size);
      printf("\n");
    } else if (choice == 4) {
      printf(BLU "\nOld base grid\n" RESET);
      print_grid(correct_grid, grid_size);
      correct_grid = generate_grid(grid_size);
      printf(GREEN "\nNew base grid generated!\n\n" RESET);
    } else if (choice == 5) {
      printf(YELLOW "\nBase grid\n" RESET);
      print_grid(correct_grid, grid_size);
    }
  } while (choice != 6);

  int **grid = get_grid_from_mask(mask, correct_grid, grid_size);

  while (!is_solved(grid, grid_size)) {
    printf("\n");
    print_grid(grid, grid_size);

    printf("\nCLUES  ");
    for (int i = 0; i < clues; i++) {
      printf(YELLOW "💡" RESET);
    }

    printf("\nLIVES  ");
    for (int i = 0; i < lives; i++) {
      printf(RED "❤️ " RESET);
    }

    int i, j, action, move;
    do {
      printf(CYN "\n\n❁ ✽ ✾ ✲ ❈ ❃ ❦ ❁ ✽ ✾ ✲ ❈ ❃ ❦\n\n"
                 "What do you want to do?\n\n"
                 " - Type 1 to make a move\n"
                 " - Type 2 to get a hint\n"
                 " - Type 3 to quit\n\n"
                 "❁ ✽ ✾ ✲ ❈ ❃ ❦ ❁ ✽ ✾ ✲ ❈ ❃ ❦\n\n" RESET);
      scanf("%d", &action);
      printf("\n");
      if (action < 1 || action > 3) {
        printf(RED "Invalid.\n" RESET);
      } else if (action == 1) {
        int i, j, move;
        get_move(&i, &j, &move, grid_size);

        grid[i][j] = move;

        if (!is_valid_grid(grid, grid_size, 1)) {
          printf(YELLOW "Invalid move: you lost a life!" RESET);
          lives--;
          grid[i][j] = -1;
          if (lives == 0) {
            printf(RED "You lost!\n" RESET);
            printf("Press any key to continue...\n");
            getchar();
            return;
          }
        } else if (grid[i][j] != correct_grid[i][j]) {
          printf(YELLOW "Though this move is valid, it might not be the right "
                        "one 🤔" RESET);
        }
      } else if (action == 2) {
        if (clues > 0) {
          int *move = find_next(grid, grid_size);
          grid[move[0]][move[1]] = correct_grid[move[0]][move[1]];
          printf(GREEN "HINT: Added %d at (%d, %d)" RESET,
                 correct_grid[move[0]][move[1]], move[0], move[1]);
          clues--;
        } else {
          printf(YELLOW "No more clues available!" RESET);
        }
      } else if (action == 3) {
        return;
      }
    } while (action < 1 || action > 3);
  }

  print_grid(grid, grid_size);
  printf(CYN "\n\nC O N G R A T U L A T I O N S  !\n\n" GREEN
             "Well done! You solved the grid 🎉🎉\n" RESET);
}

/* Solve the grid automatically when a key is pressd by the user
Copied parameters :
-int grid_size[2] : contains the size of the grid in the X and Y dimension.
*/
void autogame(int grid_size[2]) {
  int **correct_grid = generate_grid(grid_size);
  int **mask = generate_mask(grid_size);
  int **grid = get_grid_from_mask(mask, correct_grid, grid_size);

  while (!is_solved(grid, grid_size)) {
    printf("\n");
    print_grid(grid, grid_size);
    printf("\n");

    int *move = find_next(grid, grid_size);
    grid[move[0]][move[1]] = correct_grid[move[0]][move[1]];

    printf("Press any key to continue...\n");
    getchar();
  }

  printf(GREEN "The grid is solved !\n\n" RESET);
  print_grid(grid, grid_size);
}

/* Main menu that gets what the player wants to do
Copied parameters :
-int grid_size[2] : contains the size of the grid in the X and Y dimension.
*/
void menu() {
  int action_choice = 0;
  int size_choice;

  do {
    printf(MAG "\n\n      ════════════ ❀•°❀°•❀ ════════════\n"
               " Welcome to the game! What do you want to do?\n"
               "      ════════════ ❀•°❀°•❀ ════════════\n\n"
               ".・。.・゜✭・.  " BOLD UNDERLINE "Takuzu / Binero" RESET MAG
               "  .・。.・゜✭・.\n\n"
               " - Type 1 to solve a grid\n"
               " - Type 2 to solve automatically a Takuzu grid\n"
               " - Type 3 to generate a Takuzu grid\n"
               " - Type 4 to quit\n\n"
               ".・。.・゜✭・.  " BOLD UNDERLINE "Takuzu / Binero" RESET MAG
               "  .・。.・゜✭・.\n\n" RESET);

    do {
      scanf("%d", &action_choice);
      if (action_choice < 1 || action_choice > 4) {
        printf(RED "Invalid choice, please try again.\n" RESET);
      }
    } while (action_choice < 1 || action_choice > 4);

    if (action_choice != 4) {
      int grid_size[2];

      do {
        printf(CYN "\n.・。.・゜✭・.  " BOLD UNDERLINE "Grid Size" RESET CYN
                   ".・。. ・゜✭・.\n\n"
                   "What size of grid do you want?\n\n"
                   " - Type 1 for a 4x4 grid\n"
                   " - Type 2 for a 8x8 grid\n"
                   "- Type 3 for a 12x12 grid\n"
                   ".・。.・゜✭・.  " BOLD UNDERLINE "Grid Size" RESET CYN
                   "  .・。.・゜✭・.\n\n" RESET);
        scanf("%d", &size_choice);

        if (size_choice == 1) {
          grid_size[0] = 4;
          grid_size[1] = 4;
        } else if (size_choice == 2) {
          grid_size[0] = 8;
          grid_size[1] = 8;
        } else if (size_choice == 3) {
          grid_size[0] = 12;
          grid_size[1] = 12;
        } else {
          printf(RED "Invalid size choice.\n" RESET);
        }
      } while (size_choice < 1 || size_choice > 4);

      if (action_choice == 1) {
        game(grid_size);
      } else if (action_choice == 2) {
        autogame(grid_size);
      } else if (action_choice == 3) {
        printf(GREEN "\nHere is your random grid!\n" RESET);
        print_grid(generate_grid(grid_size), grid_size);
      }
    }
  } while (action_choice != 4);
}
