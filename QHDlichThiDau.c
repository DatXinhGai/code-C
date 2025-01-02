//
// Created by HP LAPTOP on 27/12/2024.
// Là bài mở rộng của bài trộm
// Còn bài nâng cao hơn là các ngày không cạnh nhau


#include <stdio.h>
#include <stdlib.h>
#include "max.h"

void lichThiDauQHD() {
    // khởi tạo
    int n, k;
    scanf("%d%d", &n, &k);
    int* lich = (int*)malloc(n*sizeof(int));
    if (!lich) {
        fprintf(stderr, "Allocated unsuccessfully!");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        scanf("%d", &lich[i]);
    }

    int kq = lich[0];
    int* chiSo = (int*)malloc(n*sizeof(int)); // mảng lưu các ngày thi đấu
    if (!chiSo) {
        fprintf(stderr, "Fail to allocate");
        exit(EXIT_FAILURE);
    }
    int dem = 0;
    chiSo[dem] = 0;


    // TH 1
    if (n <= k) {                                  // chỉ thi đấu 1 ngày
        for (int i = 0; i < n; i++) {
            if (lich[i] > kq) {
                kq = lich[i];
                chiSo[dem] = i;
            }
        }
    }

    // TH 2
    else {
        int max2 = 0;
        int *temp2 = (int*)calloc(k, sizeof(int));
        // mảng lưu các ngày trước đó có thể chọn
        if (!temp2) {
            fprintf(stderr, "Can't allocate");
            exit(EXIT_FAILURE);
        }
        for (int i = k; i < n; i++) {
            temp2[i%k] = lich[i - k];
            max2 = max(max2, lich[i - k]);
            lich[i] += max2;
            if (lich[i] > kq) {
                kq = lich[i];
                chiSo[dem] = i;
            }
        }
        free(temp2);
        temp2 = NULL;

        int max3;
        while (chiSo[dem] >= 2*k - 1) {
            dem++;
            max3 = 0;
            for (int i = k; i <= 2*k - 1; i++) {
                if (lich[chiSo[dem - 1] - i] > max3) {
                    max3 = lich[chiSo[dem - 1] - i];
                    chiSo[dem] = chiSo[dem - 1] - i;
                }
            }
        }
    }
    printf("Tiền thưởng cao nhất là: %d\n", kq);
    if (chiSo[dem] >= k) {
        int max4 = 0;
        dem++;
        for (int i = 0; i <= chiSo[dem]; i++) {
            if (lich[i] > max4) {
                max4 = lich[i];
                chiSo[dem] = i;
            }
        }
    }
    printf("Các ngày thi đấu là(bắt đầu từ ngày 0):\n");
    for (int i = dem; i >= 0; i--) {
        printf("%d ", chiSo[i]);
    }

    free(lich);
    lich = NULL;

    free(chiSo);
    chiSo = NULL;
}