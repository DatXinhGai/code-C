//
// Created by HP LAPTOP on 02/03/2025.
//
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} TOADO;

void print_Bishop(TOADO* toado, int k) {
    for (int i = 0; i < k; i++) {
        printf("%d %d\n", toado[i].x, toado[i].y);
    }
}

void Tuong(char* clen, char* cxuong, TOADO* toado,int n, int k, int count, int x, int y) {
    if (count == k) {
        print_Bishop(toado, k);
        return ;
    }
    if (x >= n) {
        return ;
    }
    if (clen[x + y] == 0 && cxuong[x - y + n - 1] == 0) {
        clen[x + y] = 1;
        cxuong[x - y + n - 1] = 1;
        toado[count].x = x;
        toado[count].y = y;
        Tuong(clen, cxuong, toado, n, k, count + 1, x + (y + 1)/n, (y + 1)%n);
        clen[x + y] = 0;
        cxuong[x - y + n - 1] = 0;
        Tuong(clen, cxuong, toado, n, k, count, x + (y + 1)/n, (y + 1)%n);
    }
}

int xepTuong() {
    int n, k;
    scanf("%d%d", &n, &k);
    TOADO* toado = (TOADO*)malloc(k*sizeof(TOADO));
    char* clen = (char*)calloc(2*n - 1, sizeof(char));
    char* cxuong = (char*)calloc(2*n - 1, sizeof(char));

    Tuong(clen, cxuong, toado, n, k, 0, 0, 0);
    return 0;
}