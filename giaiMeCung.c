//
// Created by HP LAPTOP on 27/02/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void printMaze(int** grid, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%d ", grid[i][j]);
        }
        puts("");
    }
}

bool isSafeMaze(int** grid, int row, int col, int x, int y) {
    if (x >= 0 && x < row && y >= 0 && y < col && grid[x][y] != 0 && grid[x][y] != 2) {
        return true;
    }
    return false;
}

bool solveMaze(int** grid, int row, int col, int x, int y) {
    if (x == row - 1 && y == col - 1) {
        grid[x][y] = 2;
        return true;
    }
    if (isSafeMaze(grid, row, col, x, y)) {
        grid[x][y] = 2;
        if (solveMaze(grid, row, col, x, y + 1) == true) { return true; }
        if (solveMaze(grid, row, col, x + 1, y) == true) { return true; }
        if (solveMaze(grid, row, col, x, y - 1) == true) { return true; }
        if (solveMaze(grid, row, col, x - 1, y) == true) { return true; }
        grid[x][y] = 1;
    }

    return false;
}

int giaiMeCung() {
    int row, col;
    scanf("%d%d", &row, &col);

    int** grid = (int**)malloc(row*sizeof(int*));
    for (int i = 0; i < row; i++) {
        grid[i] = (int*)malloc(col*sizeof(int));
        for (int j = 0; j < col; j++) {
            scanf("%d", &grid[i][j]);
        }
    }
    bool solvable = solveMaze(grid, row, col, 0, 0);
    if (solvable) {
        printMaze(grid, row, col);
    }
    return 0;
}