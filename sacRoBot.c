//
// Created by HP LAPTOP on 27/12/2024.
//
#include <stdio.h>
#include <stdlib.h>

// định nghĩa trạm sạc
typedef struct {
    int cach;
    int sac;
    int toiUu;
} Tram;



void sacRoBot() {
    int dung = 1;
    int t;
    scanf("%d", &t);
    for (int i = 0; i < t; i++) {
        int n;
        int l, xang;
        scanf("%d", &n);
        Tram *tram = (Tram*)malloc(n*sizeof(Tram));
        for (int j = 0; j < n; j++) {
            scanf("%d%d", &tram[j].cach, &tram[j].sac);
            tram[j].toiUu = tram[i].sac - tram[i].cach;
        }
        scanf("%d%d", &l, &xang);



        int *luu = (int*)malloc(n*sizeof(int));                 // lưu các vị trí robot vào sạc
        int dem = 0;
        int delta = l - tram[0].cach;
        int chiSo;
        if (delta > xang) {         // trường hợp biên
            dung = 0;
        } else {
            l = tram[0].cach;
            xang -= delta;
            chiSo = 0;
            int xa = xang + tram[0].sac - tram[0].cach;
            for (int j = 1; j < n; j++) {
                delta = l - tram[j].cach;
                if (delta > xang) {                         // không đủ xăng
                    if (xang + tram[chiSo].sac >= delta) {   // sạc vẫn đủ đi
                        l = tram[j].cach;
                        xang += tram[chiSo].sac - delta;
                        luu[dem++] = chiSo;                 // lưu trạm
                        chiSo = j;
                    } else {
                        dung = 0;
                        break;
                    }
                } else {
                    l = tram[j].cach;
                    xang -= delta;
                    if (xang + tram[i].toiUu > xa) {
                        chiSo = j;
                        xa = xang + tram[i].toiUu;
                    }
                }
            }
            if (xang >= tram[n - 1].cach) {

            } else if (xang + tram[chiSo].sac > tram[n - 1].cach) {
                luu[dem++] = chiSo;
            } else {
                dung = 0;
            }
        }
        if (dung) {
            printf("Có thể đến đích!\n");
            printf("Các trạm sạc đi vào là:\n");
            for (int j = 0; j < dem; j++) {
                printf("%d ", luu[j]);
            }
            printf("\n");
        } else {
            printf("Robot không đến được đích!\n");
        }
        if (tram) {
            free(tram);
            tram = NULL;
        }
        if (luu) {
            free(luu);
            luu = NULL;
        }
    }
}