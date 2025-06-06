//
// Created by HP LAPTOP on 27/02/2025.
//
#include <stdio.h>
#include <stdlib.h>

// giống hệ tọa độ descarts
int tim_mien(int i, int j, int r, int l, int quyet) {
    int x = i - r;
    int y = j - l;
    if (x < quyet && y < quyet) {
        return 2;
    }
    if (x < quyet && y >= quyet) {
        return 1;
    }
    if (x >= quyet && y < quyet) {
        return 3;
    }
    return 4;
}

void dienL_recurse(int** bang, int x, int y, int r, int l, int size, int *value) {
    int quyet = size/2;
    if (size == 2) {
        (*value)++;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                if (!bang[r + i][l + j]) {
                    bang[r + i][l + j] = *value;
                }
            }
        }
        return ;
    }


    int mien = tim_mien(x, y, r, l, quyet);
    (*value)++;
    int temp = *value;
    if (mien != 2) {
        bang[r + quyet - 1][l + quyet - 1] = temp;
        dienL_recurse(bang, r + quyet - 1, l + quyet - 1, r, l, size/2, value);
    } else {
        //(*value)++;
        dienL_recurse(bang, x, y, r, l, size/2, value);
    }

    if (mien != 1) {
        bang[r + quyet - 1][l + quyet] = temp;
        dienL_recurse(bang, r + quyet - 1, l + quyet, r, l + quyet, size/2, value);
    } else {
        //(*value)++;
        dienL_recurse(bang, x, y, r, l + quyet, size/2, value);
    }
    if (mien != 3) {
        bang[r + quyet][l + quyet - 1] = temp;
        dienL_recurse(bang, r + quyet, l + quyet - 1, r + quyet, l, size/2, value);
    } else {
        //(*value)++;
        dienL_recurse(bang, x, y, r + quyet, l,  size/2, value);
    }
    if (mien != 4) {
        bang[r + quyet][l + quyet] = temp;
        dienL_recurse(bang, r + quyet, l + quyet, r + quyet, l + quyet, size/2, value);
    } else {
        //(*value)++;
        dienL_recurse(bang, x, y, r + quyet, l + quyet, size/2 , value);
    }
}



int dienL() {
    int size;
    scanf("%d", &size);
    int x, y;
    scanf("%d%d", &x, &y);

    int** grid = (int**)malloc(size*sizeof(int*));
    for (int i = 0;i < size; i++) {
        grid[i] = (int*)calloc(size, sizeof(int));
    }
    grid[x][y] = 1;
    int value = 1;
    dienL_recurse(grid, x, y, 0, 0, size, &value);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%2d ", grid[i][j]);
        }
        puts("");
    }

    for (int i = 0; i < size; i++) {
        free(grid[i]);
        grid[i] = NULL;
    }

    free(grid);
    grid = NULL;
    return 0;
}
