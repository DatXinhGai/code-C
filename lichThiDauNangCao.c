//
// Created by HP LAPTOP on 28/12/2024.
//
#include <stdio.h>
#include <stdlib.h>

typedef struct {    // định nghĩa ngày thi đấu
    int ngay;
    int diem;
} Ngay;


void lichThiDauNangCao() {

    int n, k;
    int kq = 0;
    scanf("%d %d", &n, &k);
    Ngay *lich = (Ngay*)malloc(n*sizeof(Ngay)); // lưu các ngày thi đấu
    if (!lich) {
        fprintf(stderr, "Fail to allocate!");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        scanf("%d", &lich[i].ngay);
    }
    for (int i = 0; i < n; i++) {
        scanf("%d", &lich[i].diem);
    }
    int *chiSo = (int*)malloc(n*sizeof(int));
    if (!chiSo) {
        fprintf(stderr, "Khong the cap phat\n");
        exit(EXIT_FAILURE);
    }

    int dem = 0;
    if (lich[n - 1].ngay < k) {
        for (int i = 0; i < n; i++) {
            if (lich[i].diem > kq) {
                kq = lich[i].diem;
                chiSo[dem++] = lich[i].ngay;
            }
        }
    } else {
        int *temp = (int*)calloc(k, sizeof(int));    // lưu các giá trị của các ngày trước
        int *temp2 = (int*)calloc(k, sizeof(int));   // lưu các giá trị của k ngày đầu
        if (!temp) {
            fprintf(stderr, "Allocate unsuccessfully");
            exit(EXIT_FAILURE);
        }
        if (!temp2) {
            fprintf(stderr, "Khong the cap phat\n");
            exit(EXIT_FAILURE);
        }

        int maxt = 0;
        for (int i = 0; i < n; i++) {
            temp[lich[i].ngay % k] = temp2[lich[i].ngay % k];   // thêm vào temp đã
            temp2[lich[i].ngay % k] = lich[i].diem;             // rồi thêm vào temp2
            if (temp[lich[i].ngay % k] > maxt) {                // cập nhật maxt
                maxt = temp[lich[i].ngay % k];
            }
            lich[i].diem += maxt;                                    // cộng với max cái trước
            if (lich[i].diem > kq) {
                kq = lich[i].diem;
            }
        }
    }
    printf("%d", kq);
}