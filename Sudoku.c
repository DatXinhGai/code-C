//
// Created by HP LAPTOP on 01/03/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

void print_Sudoku(int** grid, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", grid[i][j]);
        }
        puts("");
    }
}

bool check_Sudoku(int*** zone, int** hang, int** cot, int can_n, int r, int c, int i) {
    if (hang[r][i - 1] || cot[c][i - 1] || zone[r/can_n][c/can_n][i - 1]) {
        return false;
    }
    return true;
}

bool solve_Sudoku(int** grid, int*** zone, int** hang, int** cot, int n, int can_n, int r, int c, int dem) {
    if (dem == 0) {
        return true;
    }
    if (grid[r][c]) {
        return solve_Sudoku(grid, zone, hang, cot, n, can_n, r + (c + 1)/n, (c + 1)%n, dem);
    }

    for (int i = 1; i <= n ; i++) {
        if (check_Sudoku(zone, hang, cot, can_n, r, c, i)) {
            grid[r][c] = i;
            hang[r][i - 1] = 1;
            cot[c][i - 1] = 1;
            zone[r/can_n][c/can_n][i - 1] = 1;
            if (solve_Sudoku(grid, zone, hang, cot, n, can_n, r + (c + 1)/n, (c + 1)%n, dem - 1) == true) {
                return true;
            }
            hang[r][i - 1] = 0;
            cot[c][i - 1] = 0;
            zone[r/can_n][c/can_n][i - 1] = 0;
        }
    }
    grid[r][c] = 0;
    return false;
}

int Sudoku() {
    int n;
    scanf("%d", &n);
    int can_n = (int)sqrt(n);

    int** grid = (int**)malloc(n*sizeof(int*));
    int*** zone = (int***)malloc(can_n*sizeof(int**));
    int** hang = (int**)malloc(n*sizeof(int*));
    int** cot = (int**)malloc(n*sizeof(int*));
    for (int i = 0; i < can_n; i++) {
        zone[i] = (int**)malloc(can_n*sizeof(int*));
        for (int j = 0; j < can_n; j++) {
            zone[i][j] = (int*)calloc(n, sizeof(int));
        }
    }
    for (int i = 0; i < n; i++) {
        hang[i] = (int*)calloc(n, sizeof(int));
        cot[i] = (int*)calloc(n, sizeof(int));
    }
    int dem = n*n;
    for (int i = 0; i < n; i++) {
        grid[i] = (int*)malloc(n*sizeof(int));
        for (int j = 0; j < n; j++) {
            scanf("%d", &grid[i][j]);
            if (grid[i][j]) {
                hang[i][grid[i][j] - 1] = 1;
                cot[j][grid[i][j] - 1] = 1;
                zone[i/can_n][j/can_n][grid[i][j] - 1] = 1;
                dem--;
            }
        }
    }

    if (solve_Sudoku(grid, zone, hang, cot, n, can_n, 0, 0, dem)) {
        print_Sudoku(grid, n);
    } else {
        puts("Không có lời giải");
    }

    for (int i = 0; i < can_n; i++) {
        for (int j = 0; j < can_n; j++) {
            free(zone[i][j]);
            zone[i][j] = NULL;
        }
        free(zone[i]);
        zone[i] = NULL;
    }
    free(zone);
    zone = NULL;

    for (int i = 0; i < n; i++) {
        free(grid[i]);
        grid[i] = NULL;
        free(hang[i]);
        hang[i] = NULL;
        free(cot[i]);
        cot[i] = NULL;
    }
    free(grid);
    grid = NULL;
    free(hang);
    hang = NULL;
    free(cot);
    cot = NULL;

    return 0;
}