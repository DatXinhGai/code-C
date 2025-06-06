//
// Created by HP LAPTOP on 01/03/2025.
//
#include <stdio.h>
#include <stdlib.h>

int MazeSolve(const int** maze, int row, int col, int x, int y) {
    if (x == row - 1 && y == col - 1) {
        return 1;
    }
    if (x >= row || x < 0 || y >= col || y < 0 || maze[x][y] == 0) {
        return 0;
    }
    return MazeSolve(maze, row, col, x + 1, y) + MazeSolve(maze, row, col, x, y + 1);
}

int MazeSolveDp(const int** maze, int** dp, int row, int col, int x, int y) {
    if (x < 0 || x >= row || y < 0 || y >= col || maze[x][y] == 0) {
        return 0;
    }
    if (dp[x][y]) {
        return dp[x][y];
    }
    dp[x][y] = MazeSolveDp(maze, dp, row, col , x + 1, y) + MazeSolveDp(maze, dp, row, col, x, y + 1);
    return dp[x][y];
}

int MazeCount() {
    int row, col;
    scanf("%d%d", &row, &col);

    int** maze = (int**)malloc(row*sizeof(int*));
    int** dp = (int**)malloc(row*sizeof(int*));
    for (int i = 0; i < row; i++) {
        maze[i] = (int*)malloc(col*sizeof(int));
        dp[i] = (int*)calloc(col,sizeof(int));
        for (int j = 0; j < col; j++) {
            scanf("%d", &maze[i][j]);
        }
    }

    int re = MazeSolve(maze, row, col, 0, 0);
    printf("%d\n", re);

    dp[row - 1][col - 1] = 1;
    int re1 = MazeSolveDp(maze, dp, row, col, 0, 0);
    printf("%d\n", re1);
    return 0;
}