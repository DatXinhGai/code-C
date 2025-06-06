//
// Created by HP LAPTOP on 22/02/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// có tối đa 1 ứng viên
// vì nếu không có người nào biết anh ta thì những người còn lại không biết tất cả mọi người
// thuật toán o(n) ở đây là duyệt qua n phần tử
// chọn 1 ứng viên, ở vị trí 0, chạy và tìm
// khi đó candidate < i, nếu grid[candidate][i] = 0 thì chuyển candidate sang i
// vì từ candidate + 1 đến i - 1 thì candidate biết rồi nên chắc chắn không là special

int isCandidate(int** grid, int n) {
    int candidate = 0;
    for (int i = 1; i < n; i++) {
        if (grid[candidate][i] == 1) {
            candidate = i;
        }
    }

    for (int i = 0; i < n; i++) {
        if (i != candidate) {
            if (grid[candidate][i] != 0 || grid[i][candidate] != 1) {
                return -1;
            }
        }
    }

    return candidate + 1;
}

int specialCandidate() {
    int n;
    scanf("%d", &n);
    int** grid = (int**)malloc(n*sizeof(int*));
    for (int i = 0; i < n; i++) {
        grid[i] = (int*)malloc(n*sizeof(int));
        for (int j = 0; j < n; j++) {
            scanf("%d", &grid[i][j]);
        }
    }

    int re = isCandidate(grid, n);
    printf("%d", re);

    for (int i = 0; i < n; i++) {
        free(grid[i]);
        grid[i] = NULL;
    }
    free(grid);
    grid = NULL;

    return 0;
}