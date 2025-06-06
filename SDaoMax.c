//
// Created by HP LAPTOP on 28/01/2025.
//
/*
*INPUT
5 6
1 0 0 0 0 0
0 1 0 1 1 0
1 0 0 0 0 0
1 0 0 0 1 0
1 0 0 0 1 1

OUTPUT
5
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int hang;
    int cot;
} TOADO;

typedef struct {
    TOADO* toado;
    int size;
    int max_size;
} QUANDAO;

void xetXungQuanh(int temp[], int *temp_count, int a) {
    if (a >= 0) {
        temp[(*temp_count)++] = a;
    }
}

int SDaoMax() {
    int row, col;
    scanf("%d%d", &row, &col);
    row += 2;
    col += 2;


    // tạo bảng với các cạnh toàn số 0 bao ở ngoài
    int** bang = (int**)malloc(row*sizeof(int*));
    for (int i = 0; i < row; i++) {
        bang[i] = (int*)calloc(col, sizeof(int));
    }

    for (int i = 1; i < row - 1; i++) {
        for (int j = 1; j < col - 1; j++) {
            scanf("%d", &bang[i][j]);
        }
    }


    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            bang[i][j] -= 2;
        }
    }

    //
    int max_dao = 32;
    QUANDAO *qd = (QUANDAO*)malloc(max_dao*sizeof(QUANDAO));
    int dem_dao = 0;
    for (int i = 1; i < row - 1; i++) {
        for (int j = 1; j < col - 1; j++) {
            // tìm các vị trí đất liền
            if (bang[i][j] == -1) {
                int temp[4] = {-1, -1, -1, -1};
                int temp_count = 0;

                // tìm các đảo lân cận
                xetXungQuanh(temp, &temp_count, bang[i - 1][j - 1]);
                xetXungQuanh(temp, &temp_count, bang[i - 1][j]);
                xetXungQuanh(temp, &temp_count, bang[i - 1][j + 1]);
                xetXungQuanh(temp, &temp_count, bang[i][j - 1]);
                if (temp_count >= 2) {
                    int trung = 1;
                    for (int k = 1; k < temp_count; k++) {
                        if (temp[k] != temp[0]) {
                            trung = 0;
                            temp[1] = temp[k];
                            temp_count = 2;
                            break;
                        }
                    }
                    if (trung) {
                        temp_count = 1;
                    }
                }



                // tạo đảo mới
                if (temp_count == 0) {
                    bang[i][j] = dem_dao;
                    qd[dem_dao].size = 1;
                    qd[dem_dao].max_size = 32;
                    qd[dem_dao].toado = (TOADO*)malloc(qd[dem_dao].max_size*sizeof(TOADO));
                    qd[dem_dao].toado[0].hang = i;
                    qd[dem_dao].toado[0].cot = j;
                    dem_dao++;
                    if (dem_dao == max_dao) {
                        max_dao <<= 1;
                        qd = (QUANDAO*)realloc(qd, max_dao*sizeof(QUANDAO));
                    }
                } else if (temp_count == 1) {   // có 1 quần đảo lân cận thì sát nhập luôn vào
                    int chiSo = temp[0];
                    bang[i][j] = chiSo;
                    qd[chiSo].toado[qd[chiSo].size].hang = i;
                    qd[chiSo].toado[qd[chiSo].size].cot = j;
                    qd[chiSo].size++;
                    // mở rộng size
                    if (qd[chiSo].size >= qd[chiSo].max_size) {
                        qd[chiSo].max_size <<= 1;
                        qd[chiSo].toado = (TOADO*)realloc(qd[chiSo].toado, qd[chiSo].max_size*sizeof(TOADO));
                    }
                } else {
                    // trường hợp có 2 đảo kề cần xem đảo nào chỉ số thấp hơn thì sát nhập về đảo đó
                    if (temp[0] > temp[1]) {    // temp[0] là đảo bé hơn
                        temp[0] += temp[1];
                        temp[1] = temp[0] - temp[1];
                        temp[0] -= temp[1];
                    }

                    int csNho = temp[0];
                    int csTo = temp[1];

                    // thêm thằng vừa rồi vào csnho
                    bang[i][j] = csNho;
                    qd[csNho].toado[qd[csNho].size].hang = i;
                    qd[csNho].toado[qd[csNho].size].cot = j;
                    qd[csNho].size++;


                    // tiến hành copy vào đảo chỉ số nhỏ
                    qd[csNho].size += qd[csTo].size;
                    // resize nếu không đủ chỗ
                    if (qd[csNho].size >= qd[csNho].max_size) {
                        qd[csNho].max_size <<= 1;
                        qd[csNho].toado = (TOADO*)realloc(qd[csNho].toado, qd[csNho].max_size*sizeof(TOADO));
                    }
                    // không trùng thì dùng memcpy cho nhanh
                    memcpy(qd[csNho].toado + qd[csNho].size, qd[csTo].toado, qd[csTo].size*sizeof(TOADO));

                    // đặt lại những thằng trong cs to về cs nhỏ và giải phóng cs to
                    for (int k = 0; k < qd[csTo].size; k++) {
                        bang[qd[csTo].toado[k].hang][qd[csTo].toado[k].cot] = csNho;
                    }
                    qd[csTo].size = 0;
                    free(qd[csTo].toado);
                    qd[csTo].toado = NULL;

                }
            }
        }
    }

    int kq = 0;
    for (int i = 0; i < dem_dao; i++) {
        if (qd[i].size > kq) {
            kq = qd[i].size;
        }
    }

    printf("%d", kq);

    for (int i = 0; i < dem_dao; i++) {
        if (qd[i].size > 0) {
            free(qd[i].toado);
            qd[i].toado = NULL;
        }
    }
    free(qd);
    qd = NULL;

    for (int i = 0; i < row; i++) {
        free(bang[i]);
        bang[i] = NULL;
    }
    free(bang);
    bang = NULL;

    return 0;
}