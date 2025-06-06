//
// Created by HP LAPTOP on 02/03/2025.
//
#include <stdio.h>
#include <stdlib.h>

int maxMaze(int a, int b) {
    return (a > b)? a : b;
}

// độ phức tạp cao nhất là 4^(m*n)
int longMaze_re(int** maze, int row, int col, int x, int y) {
    if (x == row - 1 && y == col - 1) {
        return 1;
    }
    if (x < 0 || x >= row || y < 0 || y >= col || maze[x][y] == 0 || maze[x][y] == -1) {
        return 0;
    }
    maze[x][y] = -1;
    int re = maxMaze(maxMaze(longMaze_re(maze, row, col, x + 1, y), longMaze_re(maze, row, col, x - 1, y)),
                    maxMaze(longMaze_re(maze, row, col, x, y - 1), longMaze_re(maze, row, col, x, y + 1)));
    maze[x][y] = 1;
    if (re == 0) {
        return 0;
    }
    return re + 1;
}
// những thằng nào cạnh 2 số -1 thì chắc chắn không chọn
// vì nếu đi xuống như vậy thì chắc chắn thừa bước
int shortMaze_re(int** maze, int row, int col, int x, int y) {
    if (x == row - 1 && y == col - 1) {
        return 1;
    }
    if (x < 0 || x >= row || y < 0 || y >= col || maze[x][y] == 0 || maze[x][y] == -1) {
        return 0;
    }
    maze[x][y] = -1;
    int huong[4];
    int cs = 0;
    int min = row*col + 1;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (abs(i - j) == 1) {
                huong[cs] = shortMaze_re(maze, row, col, x + i, y + j);
                if (huong[cs] > 0) {
                    if (huong[cs] < min) {
                        min = huong[cs];
                    }
                }
                cs++;
            }
        }
    }
    maze[x][y] = 1;
    if (min > row*col) {
        return 0;
    }

    return min + 1;
}

//qhd thường dùng để tìm đường đi min
int shortMaze_dp(int** dp, int row, int col) {
    for (int i = col - 2; i >= 0; i--) {
        if (dp[row - 1][i] == 0) {
            if (dp[row - 1][i + 1] > 0) {
                dp[row - 1][i] = dp[row - 1][i + 1] + 1;
            } else {
                dp[row - 1][i] = -1;
            }
        }
    }

    for (int i = row - 2; i >= 0; i--) {
        for (int j = col - 1; j >= 0; j--) {
            if (dp[i][j] == 0) {
                int temp[2];
                int cs = 0;
                if (i < row - 1 && dp[i + 1][j] > 0) {
                    temp[cs++] = dp[i + 1][j];
                }
                if (j < col - 1 && dp[i][j + 1] > 0) {
                    temp[cs++] = dp[i][j + 1];
                }
                if (cs == 0) {
                    dp[i][j] = -1;
                } else if (cs == 1) {
                    dp[i][j] = temp[0] + 1;
                } else {
                    dp[i][j] = ((temp[0] < temp[1])? temp[0] : temp[1]) + 1;
                }
            }
        }
    }
    return dp[0][0];
}

int longMaze() {
    int row, col;
    scanf("%d %d", &row, &col);

    int** maze = (int**)malloc(row*sizeof(int*));
    int** dp = (int**)malloc(row*sizeof(int*));
    for (int i = 0; i < row; i++) {
        maze[i] = (int*)malloc(col*sizeof(int));
        dp[i] = (int*)malloc(col*sizeof(int));
        for (int j = 0; j < col; j++) {
            scanf("%d", &maze[i][j]);
            dp[i][j] = maze[i][j] - 1;
        }
    }
    dp[row - 1][col - 1] = 1;
    int re = longMaze_re(maze, row, col, 0, 0);
    int min = shortMaze_re(maze, row, col, 0, 0);
    int hay = shortMaze_dp(dp, row, col);
    if (re == 0) {
        puts("Không có cách nào đến đích");
    } else {
        printf("%d %d %d\n", re, min, hay);
    }

    for (int i = 0; i < row; i++) {
        free(dp[i]);
        free(maze[i]);
    }
    free(dp);
    free(maze);

    return 0;
}