#ifndef RULES_FILE
#define RULES_FILE

int no_redundant_row_column(int **grid, int grid_size[2]);
int is_valid_row_column(int *row_columnn, int row_column_size);
int is_valid_grid(int **grid, int grid_size[2], int verbose);
int is_solved(int **grid, int grid_size[2]);

#endif