//
// Created by HP LAPTOP on 26/02/2025.
//
#include <stdio.h>
#include <stdlib.h>

int nCkCoNho(int n, int k, int** bang) {
    if (k == 0 || k == n) {
        bang[n][k] = 1;
        return bang[n][k];
    }

    if (bang[n][k] > 0) {
        return bang[n][k];
    }

    bang[n][k] = nCkCoNho(n - 1, k, bang) + nCkCoNho(n - 1, k - 1, bang);
    return bang[n][k];
}

int toHop() {
    int n, k;
    scanf("%d%d", &n, &k);

    int** bang = (int**)malloc((n + 1)*sizeof(int*));
    for (int i = 0; i < n + 1; i++) {
        bang[i] = (int*)calloc(n + 1, sizeof(int));
    }
    bang[0][0] = 1;

    int re = nCkCoNho(n, k, bang);
    printf("%d", re);

    return 0;
}