//
// Created by HP LAPTOP on 01/01/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "lamTron.h"
#include <string.h>


bool delta(double x) {
    return (fabs(x - (int)x) < 0.001)? true : false;

}

void inRaNghiem(int n, double* nghiem, bool vn, bool vsn) {
    if (vn) {
        printf("Hệ đã nhập vô nghiệm\n");
    } else if (vsn) {
        printf("Hệ đã nhập có vô số nghiệm\n");
    } else {
        printf("Hệ có nghiệm:\n");
        for (int i = 0; i < n; i++) {
            if (delta(nghiem[i])) {
                printf("x%d = %d\n", i + 1, lamTron(nghiem[i]));
            } else {
                printf("x%d = %lf\n", i + 1, nghiem[i]);
            }
        }
    }
}


// lúc nào cũng nhanh hơn
// tốt nhất là chạy ngược về để khỏi pivot với factor, nhớ vào
void Gauss_Jordan(int n, double** he, double* nghiem)
{
    for (int i = 0; i < n; i++) {
        bool dung = false;
        for (int j = i; j < n; j++) {
            if (he[j][i] != 0) {
                if (i != j) {
                    double* temp = he[i];                       // đổi con trỏ bản chất là chuyển vị trí mảng
                    he[i] = he[j];
                    he[j] = temp;
                }
                dung = true;
                break;
            }
        }
        if (dung) {
            double pivot = he[i][i];        // he[i][i] bị thay đổi ngay ban đầu dẫn đến sai sót
            for (int j = i; j < n + 1; j++) {
                he[i][j] /= pivot;           // đưa hệ số đầu về 1
            }
            for (int k = 0; k < n; k++) {
                if (k != i) {
                    double factor = he[k][i];
                    for (int j = i; j < n + 1; j++) {
                        he[k][j] -= factor * he[i][j];
                        // he[k][j] -= he[k][i] * he[i][j]; sai vì bằng 0 rồi              // do hệ số bên trên là 1
                    }
                }
            }
        }
    }
    bool vsn = false;
    bool vn = false;
    for (int i = 0; i < n; i++) {
        if (he[i][i] == 0 && he[i][n] != 0) {
            vn = true;
            break;
        } else if (he[i][i] == 0 && he[i][n] == 0) {
            vsn = true;
        } else {
            nghiem[i] = he[i][n];
        }
    }
    inRaNghiem(n, nghiem, vn, vsn);
}



double tinhDet(int cot, int n, double** he) {
    double **hang = (double**)malloc(n*sizeof(double*));
    double det = 1;
    for (int i = 0; i < n; i++) {
        int dem = 0;
        *(hang + i) = (double*)malloc(n*sizeof(double));

        /* không được gán thẳng mảng vì là tham chiếu
         * phải copy từng cái
         * có thể dùng memcpy thay thế vì có cùng số phần tử bằng cách
         * memcpy(hang[i], he[i], n*sizeof(double))
         * hoặc memmove cũng được và memcpy nhanh hơn
         * thay cột j bằng cột n và tính det(Aj)/det(A) mà không cần quan tâm dấu
         */
        for (int j = 0; j < n; j++) {
            if (dem == cot) { dem++; }
            hang[i][j] = he[i][dem];
            dem++;
        }
    }
    for (int i = 0; i < n; i++) {
        bool dung = false;
        for (int j = i; j < n; j++) {
            if (hang[j][i] != 0) {
                dung = true;
                if (i == j) {
                    double* temp = hang[i];
                    hang[i] = hang[j];
                    hang[j] = temp;
                }
            }
        }
        if (dung) {
            det *= hang[i][i];
            for (int j = i + 1; j < n; j++) {
                for (int k = n - 1; k >= i; k--) {
                    hang[j][k] -= hang[j][i] / hang[i][i] * hang[i][k];
                }
            }
        } else {
            det = 0;
            break;
        }
    }

    for (int i = 0; i < n; i++) {
        free(hang[i]);
        hang[i] = NULL;
    }

    free(hang);
    hang = NULL;

    det = pow(-1, n - cot + 1) * det;
    if (cot == n) {
        det *= - 1;
    }
    return det;
}



double tinhDet2(int cot, int n, double** he) {
    double det = 1;
    double **hang = (double**)malloc(n*sizeof(double*));
    for (int i = 0; i < n; i++) {
        *(hang + i) = (double*)malloc(n*sizeof(double));
        memcpy(hang[i], he[i], n*sizeof(double));       // hoặc phải là memcpy(*(hang + i), *(he + i), n*sizeof(double));
        if (cot != n) {
            hang[i][cot] = he[i][n];
        }
    }

    for (int i = 0; i < n; i++) {
        bool dung = false;
        for (int j = i; j < n; j++) {
            if (hang[j][i] != 0) {
                dung = true;
                if (i != j) {
                    double *temp = hang[i];
                    hang[i] = hang[j];
                    hang[j] = temp;
                }
                break;
            }
        }
        if (dung) {
            det *= hang[i][i];
            for (int k = i + 1; k < n; k++) {
                for (int j = n - 1; j >= i; j--) {
                    hang[k][j] -= hang[k][i]/hang[i][i] * hang[i][j];
                }
            }
        } else {
            det = 0;
            for (int k = 0; k < n; k++) {
                free(hang[k]);
                hang[k] = NULL;
            }
            free(hang);
            hang = NULL;

            return det;
        }
    }
    for (int i = 0; i < n; i++) {
        free(hang[i]);
        hang[i] = NULL;
    }
    free(hang);
    hang = NULL;

    return det;
}



void Cramer(int n, double** he, double* nghiem) {
    bool vn = false;
    bool vsn = false;

    double* Det = (double*)malloc((n + 1)*sizeof(double));

    for (int i = 0; i < n + 1; i++) {
        Det[i] = tinhDet2(i, n, he);

    }


    if (Det[n] == 0) {
        for (int i = 0; i < n; i++) {
            if (Det[i] != 0) {
                vn = true;
                break;
            }
        }
        if (!vn) {
            vsn = true;
        }

    } else {
        for (int i = 0; i < n; i++) {
            nghiem[i] = Det[i]/Det[n];
        }
    }

    free(Det);
    Det = NULL;

    inRaNghiem(n, nghiem, vn, vsn);
}


void hePhuongTrinh() {
    int tinhTiep;
    int n;
    do {
        printf("Nhập số ẩn: ");
        scanf("%d", &n);
        double **he = (double**)malloc(n*sizeof(double*));
        double *nghiem = (double*)malloc(n*sizeof(double));
        for (int i = 0; i < n; i++) {
            *(he + i) = (double*)malloc((n + 1)*sizeof(double));
        }
        printf("Nhập như casio:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n + 1; j++) {
                scanf("%lf", &he[i][j]);
            }
        }
        printf("\n");
        printf("--------------------------\n");


        //Gauss_Jordan(n, he, nghiem);
        Cramer(n, he, nghiem);



        printf("Tính tiếp?(1/0): ");
        scanf("%d", &tinhTiep);
        for (int i = 0; i < n; i++) {
            free(he[i]);
            he[i] = NULL;
        }
        free(he);
        he = NULL;
        free(nghiem);
        nghiem = NULL;
    } while (tinhTiep);
}