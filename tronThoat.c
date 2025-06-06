//
// Created by HP LAPTOP on 29/01/2025.
//
/*
INPUT
5 6
1 1
1 0 0 0 0 0
0 1 0 1 1 0
1 0 0 0 0 0
1 0 0 0 1 0
1 0 0 0 1 1

OUTPUT
0

INPUT
5 6
1 1
1 0 0 0 0 0
0 1 0 1 1 0
1 1 0 0 0 0
1 0 0 0 1 0
1 0 0 0 1 1

OUTPUT
1
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int hang;
    int cot;
} TOADO;

typedef struct {
    TOADO* toado;
    int size;
    int max_size;
} PATH;

void xetXungQuanhtronThoat(int temp[], int *temp_count, int a) {
    if (a >= 0) {
        temp[(*temp_count)++] = a;
    }
}

int tronThoat() {
    int row, col;
    int x, y;

    scanf("%d%d", &row, &col);
    scanf("%d%d", &x, &y);

    // mở rộng bảng bao quanh bởi toàn -2;
    row += 2;
    col += 2;
    x++;
    y++;

    int **bang = (int**)malloc(row*sizeof(int*));
    for (int i = 0; i < row; i++) {
        bang[i] = (int*)calloc(col, sizeof(int));
    }

    for (int i = 1; i < row - 1; i++) {
        for (int j = 1; j < col - 1; j++) {
            scanf("%d", &bang[i][j]);
        }
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            bang[i][j] -= 2;
        }
    }

    // tạo các path
    int max_path = 32;
    PATH* path = (PATH*)malloc(max_path*sizeof(PATH));
    int dem_path = 0;

    for (int i = 1; i < row - 1; i++) {
        for (int j = 1; j < col - 1; j++) {
            if (bang[i][j] == -1) {
                int temp[2] = {-1, -1};
                int temp_count = 0;

                xetXungQuanhtronThoat(temp, &temp_count, bang[i][j - 1]);
                xetXungQuanhtronThoat(temp, &temp_count, bang[i - 1][j]);
                if (temp_count == 2) {	// trùng nhau
                    if (temp[0] == temp[1]) {
                        temp_count--;
                    }
                }

                if (temp_count == 0) {	// tạo path mới
                    bang[i][j] = dem_path;
                    path[dem_path].size = 1;
                    path[dem_path].max_size = 32;
                    path[dem_path].toado = (TOADO*)malloc(path[dem_path].max_size*sizeof(PATH));
                    path[dem_path].toado[0].hang = i;
                    path[dem_path].toado[0].cot = j;
                    dem_path++;
                    if (dem_path == max_path) {
                        max_path <<= 1;
                        path = (PATH*)realloc(path, max_path*sizeof(PATH));
                    }
                } else if (temp_count == 1) {    // chỉ có 1 path xung quanh

                    int chiSo = temp[0];
                    bang[i][j] = chiSo;
                    path[chiSo].toado[path[chiSo].size].hang = i;
                    path[chiSo].toado[path[chiSo].size].cot = j;
                    path[chiSo].size++;
                    if (path[chiSo].size >= path[chiSo].max_size) {
                        path[chiSo].max_size <<= 1;
                        path[chiSo].toado = (TOADO*)realloc(path[chiSo].toado, path[chiSo].max_size*sizeof(TOADO));
                    }
                } else {    // có 2 path thì chọn path nhỏ hơn
                    if (temp[0] > temp[1]) {
                        temp[0] += temp[1];
                        temp[1] = temp[0] - temp[1];
                        temp[0] -= temp[1];
                    }

                    int csNho = temp[0];
                    int csTo = temp[1];

                    // cập nhật điểm đang xét
                    bang[i][j] = csNho;
                    path[csNho].toado[csNho].hang = i;
                    path[csNho].toado[csNho].cot = j;
                    path[csNho].size++;

                    // cập nhật csto vào cs nhỏ

                    // thay đổi các giá trị trong bảng
                    for (int k = 0; k < path[csTo].size; k++) {
                        bang[path[csTo].toado[k].hang][path[csTo].toado[k].cot] = csNho;
                    }
                    // copy các toado của csto vào csnho
                    memcpy(path[csNho].toado + path[csNho].size, path[csTo].toado, path[csTo].size);

                    path[csNho].size += path[csTo].size;
                    path[csTo].size = 0;
                    while (path[csNho].size >= path[csNho].max_size) {
                        path[csNho].max_size <<= 1;
                        path[csNho].toado = (TOADO*)realloc(path[csNho].toado, path[csNho].max_size*sizeof(TOADO));
                    }
                }
            }
        }
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%2d ", bang[i][j]);
        }
        puts("");
    }
    int cs = bang[x][y];
    int thoat = 0;
    for (int i = 0; i < path[cs].size; i++) {
        if (path[cs].toado[i].hang == 1 ||
            path[cs].toado[i].hang == row - 2 ||
            path[cs].toado[i].cot == 1 ||
            path[cs].toado[i].cot == col - 2) {
            thoat = 1;
            break;
        }
    }

    if (thoat) {
        puts("1");
    } else {
        puts("0");
    }



    for (int i = 0; i < dem_path; i++) {
        free(path[i].toado);
        path[i].toado = NULL;
    }
    free(path);
    path = NULL;

    for (int i = 0; i < row; i++) {
        free(bang[i]);
        bang[i] = NULL;
    }
    free(bang);
    bang = NULL;

    return 0;

}