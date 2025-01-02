//
// Created by HP LAPTOP on 27/12/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include "min.h"
#include "max.h"

void suoiAm() {
    int k, n;
    scanf("%d %d", &n, &k);
    int *nha = (int*)malloc(n*sizeof(int));
    int *suoi = (int*)malloc(k*sizeof(int));
    for (int i = 0; i < n; i++) {
        scanf("%d", &nha[i]);
    }
    for (int i = 0; i < k; i++) {
        scanf("%d", &suoi[i]);
    }

    int r = abs(nha[0] - suoi[0]);
    if (k == 1) {
        r = min(abs(nha[0] - suoi[0]), abs(nha[n - 1] - suoi[0]));
    } else {
        int dem = 0;
        for (int i = 0; i < n; i++) {
            if (dem == k - 1) {
                r = max(r, nha[n - 1] - suoi[dem]);
            } else if (nha[i] < suoi[dem]) {
                r = max(r, suoi[dem] - nha[i]);
            } else if (nha[i] > suoi[dem + 1]) {
                while (dem < k - 1 && nha[i] > suoi[dem + 1]) {
                    dem++;
                }
                r = max(r, min(nha[i] - suoi[dem], suoi[dem + 1] - nha[i]));
            } else {
                r = max(r, min(nha[i] - suoi[dem], suoi[dem + 1] - nha[i]));
            }
        }
    }
    printf("%d", r);

    free(nha);
    nha = NULL;
    free(suoi);
    suoi = NULL;
}


