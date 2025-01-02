//
// Created by HP LAPTOP on 27/12/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include "min.h"


void QHDduongDiMin() {
    int m, n;
    scanf("%d%d", &m, &n);
    int** bang = (int**)malloc(m*sizeof(int*));
    for (int i = 0; i < m; i++) {
        *(bang + i) = (int*)malloc(n*sizeof(int));
        for (int j = 0; j < n; j++) {
            scanf("%d", &bang[i][j]);
        }
    }



    for (int j = 1; j < n; j++) {
        for (int i = 0; i < m; i++) {
            if (i == 0) {
                bang[i][j] += min(bang[i][j - 1], bang[i + 1][j - 1]);
            } else if (i == m - 1) {
                bang[i][j] += min(bang[i][j - 1], bang[i - 1][j - 1]);
            } else {
                bang[i][j] += min(bang[i][j - 1], min(bang[i - 1][j - 1], bang[i + 1][j - 1]));
            }
        }
    }
    int* chiSo = (int*)malloc(n*sizeof(int));
    int cot = n - 1;
    int nho;
    int hang = 0;

    int kq = bang[0][n - 1];
    for (int i = 0; i < m; i++) {
        if (bang[i][n - 1] < kq) {
            kq = bang[i][n - 1];
            hang = i;
        }
    }
    chiSo[cot--] = hang;
    while (cot >= 0) {
        if (hang == 0) {
            nho = min(bang[hang][cot], bang[hang + 1][cot]);
            if (nho == bang[hang][cot]) {
                chiSo[cot--] = hang;
            } else {
                chiSo[cot--] = ++hang;
            }
        } else if (hang == m - 1) {
            nho = min(bang[hang][cot], bang[hang - 1][cot]);
            if (nho == bang[hang][cot]) {
                chiSo[cot--] = hang;
            } else {
                chiSo[cot--] = --hang;
            }
        } else {
            nho = min(bang[hang][cot], min(bang[hang - 1][cot], bang[hang + 1][cot]));
            if (nho == bang[hang][cot]) {
                chiSo[cot--] = hang;
            } else if (nho == bang[hang - 1][cot]) {
                chiSo[cot--] = --hang;
            } else {
                chiSo[cot--] = ++hang;
            }
        }
    }


    printf("Đường đi ngắn nhất là: %d\n", kq);
    printf("Các hàng đã đi qua là: \n");
    for (int j = 0; j < n; j++) {
        printf("%d ", chiSo[j]);
    }


    for (int i = 0; i < m; i++) {
        free(bang[i]);
        bang[i] = NULL;
    }
    free(bang);
    bang = NULL;
    free(chiSo);
    chiSo = NULL;

}