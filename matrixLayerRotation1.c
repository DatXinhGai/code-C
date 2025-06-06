//
// Created by HP LAPTOP on 07/02/2025.
//
#include <stdio.h>
#include <stdlib.h>
/*
 * thực hiện việc xoay từng lớp trên mảng
 * giống trò chơi xếp hình mà 1 ô bị trống
 * lưu 1 phần tử đầu là temp đẩy ngược cách hàng và cột rồi trả lại
 */

void rotate(int** grid, int r, int c, int n, int i) {
    int tren = i;
    int trai = i;
    int duoi = r - 1 - i;
    int phai = c - 1 - i;
    int perimeter = (duoi - tren + phai - trai)*2;
    int rotation = n%perimeter;



    while (rotation--) {
        int temp = grid[tren][trai];
        for (int col = trai; col < phai; col++) {
            grid[tren][col] = grid[tren][col + 1];
        }
        for (int row = tren; row < duoi; row++) {
            grid[row][phai] = grid[row + 1][phai];
        }
        for (int col = phai; col > trai; col--) {
            grid[duoi][col] = grid[duoi][col - 1];
        }
        for (int row = duoi; row > tren; row--) {
            grid[row][trai] = grid[row - 1][trai];
        }
        grid[i + 1][i] = temp;
    }
}

int matrixLayerRotation1() {
    int r, c, n;
    scanf("%d%d%d", &r, &c, &n);
    int** grid = (int**)malloc(r*sizeof(int*));
    for (int i = 0; i < r; i++) {
        grid[i] = (int*)malloc(c*sizeof(int));
        for (int j = 0; j < c; j++) {
            scanf("%d", &grid[i][j]);
        }
    }

    int layers = (r < c)? (r/2 + r%2) : (c/2 + c%2);
    for (int i = 0 ; i < layers; i++) {
        rotate(grid, r, c, n, i);
    }

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            printf("%d ", grid[i][j]);
        }
        puts("");
        free(grid[i]);
        grid[i] = NULL;
    }

    free(grid);
    grid = NULL;

    return 0;

}