#include "logic.h"

#include <stdlib.h>

#include "constants.h"

int ** init_grid(int width, int height) {
    int ** grid = (int **)malloc(sizeof(int *) * width);
    for (int i = 0; i < width; i++) {
        grid[i] = (int *)malloc(sizeof(int) * height);
    }
    return grid;
}

void free_grid(int **grid) {
    int i = 0;
    while (grid[i]) {
        free(grid[i]);
        i++;
    }
    free(grid);
}
