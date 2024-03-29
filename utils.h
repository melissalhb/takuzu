#ifndef UTILS_FILE
#define UTILS_FILE

int is_same(int *arr1, int *arr2, int arr_size);
int *get_column(int **grid, int col_idx, int col_size);
int **create_grid(int grid_size[2], int initial_value);
int **get_grid_from_mask(int **mask, int **original_grid, int grid_size[2]);
void print_grid(int **grid, int grid_size[2]);
int **generate_mask(int grid_size[2]);
int **generate_mask_from_user(int grid_size[2]);

#endif