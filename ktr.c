//
// Created by HP LAPTOP on 07/01/2025.
//

#include <stdio.h>
#include <math.h>
void tinhLuong() {
    const int c1 = 100000;
    const int c2 = 130000;
    const int an = 50000;
    int tien = 0;
    int hs, ms, he, me;

    scanf("%d:%d", &hs, &ms);
    scanf("%d:%d", &he, &me);
    hs = hs*100 + ms;
    he = he*100 + me;

    if (he < hs) {
        puts("-1");
    } else if (he == hs) {
        puts("0");
    } else {
        if (he > 1800 && hs < 1800) {
            tien = (1800 - hs)/100*c1 + (he - 1800)/100*c2;
        } else if (he <= 1800) {
            tien = (he - hs)/100*c1;
        } else {
            tien = (he - hs)/100*c2;
        }
    }
    if (hs <= 1200 && he > 1300) {
        tien += an;
    }
    printf("%d", tien);
}


void  thuRac() {
    int mang[100000];
    int dem = 0;
    do {
        scanf("%d", &mang[dem]);
    } while (mang[dem++] != 0);
    dem -= 1;
    double tra[7] = {0, 0.25, 0.2, 0.4};

}