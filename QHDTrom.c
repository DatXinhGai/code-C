//
// Created by HP LAPTOP on 27/12/2024.
//

/* SPIRIT là bắt đầu từ nhà thứ 3 + nhà 1, nhà thứ 4 sẽ cộng thêm max của nhà 1 và nhà 2,
 * 5 thì 2 và 3 và cứ thế
 * đến cuối so sánh nhà n và nhà n - 1
 */
#include <stdio.h>
#include <stdlib.h>
#include "max.h"


void TromQHD() {
    int n;
    int kq = 0;
    scanf("%d", &n);
    int *tien = (int*)malloc(n*sizeof(int));
    int *chiSo = (int*)malloc(n*sizeof(int));
    int dem = 0;
    for (int i = 0; i < n; i++) {
        scanf("%d", &tien[i]);
    }
    if (n == 1) {
        kq = tien[0];
        printf("%d\n", kq);
    } else if (n == 2) {
        kq = max(tien[0], tien[1]);
        printf("%d\n", kq);
    } else {
        tien[2] += tien[0];
        for (int i = 3; i < n; i++) {
            tien[i] += max(tien[i - 3], tien[i - 2]);
        }
        kq = max(tien[n - 1], tien[n - 2]);
        printf("%d\n", kq);
    }

    if (kq == tien[n - 1]) {
        chiSo[dem] = n - 1;
    } else {
        chiSo[dem] = n - 2;
    }
    while (chiSo[dem] >= 3) {
        if (tien[chiSo[dem] - 2] > tien[chiSo[dem] - 3]) {
            chiSo[++dem] = chiSo[dem] - 2;
        } else {
            chiSo[++dem] = chiSo[dem - 3];
        }
    }
    if (chiSo[dem] == 2) {
        chiSo[++dem] = 0;
    }

    printf("Các nhà đã trộm là:\n");
    for (int i = dem; i >= 0; i--) {
        printf("%d ", chiSo[i]);
    }

    free(chiSo);
    chiSo = NULL;
    free(tien);
    tien = NULL;
}