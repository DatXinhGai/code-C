//
// Created by HP LAPTOP on 31/12/2024.
//
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "lamTron.h"



typedef struct {
    double x;
    int boi;
} Nghiem;


double f(double x , double a, double b, double c, double d) {
    return a*pow(x, 3) + b*x*x + c*x + d;
}

double fphay(double x, double a, double b, double c) {
    return 3*a*x*x + 2*b*x + c;
}

double tuyetDoi(double x) {
    return fabs(x);
}

double xapXiNewton(double x, double a, double b, double c, double d) {
    for (int i = 0; i < 200; i++) {
        x = x - f(x, a, b, c, d)/fphay(x, a, b, c);
    }
    return x;
}

double timDiemUon(double a, double b) {
    return -b/3*a;
}

void xuLiDiemUon(double diemUon, double a, double b, double c, double d, double delta, Nghiem* nghiem, int *dem) {
    if (tuyetDoi(f(diemUon, a, b, c, d)) < 0.0001) {
        nghiem[*dem].x = diemUon;
        nghiem[(*dem)++].boi = 1;

    } else if (f(diemUon, a, b, c, d) > 0.0001) {
        nghiem[*dem].x = xapXiNewton(diemUon - 0.0001, a, b, c, d);
        nghiem[(*dem)++].boi = 1;
    } else {
        nghiem[*dem].x = xapXiNewton(diemUon + 0.0001, a, b, c, d);
        nghiem[(*dem)++].boi = 1;
    }
}




double tinhDelta(double a, double b, double c) {
    return b*b - 3*a*c;
}




void ptBac3() {
    double a, b, c, d;
    Nghiem *nghiem = (Nghiem*)malloc(3*sizeof(Nghiem));
    int traLoi;

    do {
        printf("Nhập a, b, c, d:\n");
        scanf("%lf%lf%lf%lf", &a, &b, &c, &d);
        double delta = tinhDelta(a, b, c);
        double diemUon = timDiemUon(a, b);

        int dem = 0;
        if (delta < 0) {
            xuLiDiemUon(diemUon, a, b, c, d, delta, nghiem, &dem);
        } else {
            double ct1 = (-b - sqrt(delta))/3/a;
            double ct2 = -2*b/3/a - ct1;
            if (tuyetDoi(f(ct1, a, b, c, d)) < 0.0001) {
                nghiem[dem].x = ct1;
                nghiem[dem++].boi = 2;
                nghiem[dem].x = xapXiNewton(ct2 + 0.0001, a, b, c, d);
                nghiem[dem++].boi = 1;
            } else if (tuyetDoi(f(ct2, a, b, c, d)) < 0.001) {
                nghiem[dem].x = xapXiNewton(ct1 - 0.0001, a, b, c, d);
                nghiem[dem++].boi = 1;
                nghiem[dem].x = ct2;
                nghiem[dem++].boi = 2;
            } else if (f(ct1, a, b, c, d)*f(ct2, a, b, c, d) < 0) {
                nghiem[dem].x = xapXiNewton(ct1 - 0.0001, a, b, c, d);
                nghiem[dem++].boi = 1;
                xuLiDiemUon(diemUon, a, b, c, d, delta, nghiem, &dem);
                nghiem[dem].x = xapXiNewton(ct2 + 0.0001, a, b, c, d);
                nghiem[dem++].boi = 1;
            } else if (f(diemUon, a, b, c, d) > 0) {
                if (f(ct1, a, b, c, d) > f(ct2, a, b, c, d)) {
                    nghiem[dem].x = xapXiNewton(ct1 - 0.0001, a, b, c, d);
                    nghiem[dem++].boi = 1;
                } else {
                    nghiem[dem].x = xapXiNewton(ct2 + 0.0001, a, b, c, d);
                    nghiem[dem++].boi = 1;
                }
            } else {
                if (f(ct1, a, b, c, d) > f(ct2, a, b, c, d)) {
                    nghiem[dem].x = xapXiNewton(ct2 + 0.0001, a, b, c, d);
                    nghiem[dem++].boi = 1;
                } else {
                    nghiem[dem].x = xapXiNewton(ct1 - 0.0001, a, b, c, d);
                    nghiem[dem++].boi = 1;
                }
            }
        }
        printf("\n");
        for (int i = 0; i < dem; i++) {
            double lech = tuyetDoi(nghiem[i].x - lamTron(nghiem[i].x));
            if (lech <= 0.0001) {
                printf("x%d = %d    %d\n", i + 1, lamTron(nghiem[i].x), nghiem[i].boi);
            } else {
                printf("x%d = %lf   %d\n", i + 1, nghiem[i].x, nghiem[i].boi);
            }

        }
        printf("Tính tiếp?(1/0): ");
        scanf("%d", &traLoi);
    } while (traLoi);

    free(nghiem);
    nghiem = NULL;
}