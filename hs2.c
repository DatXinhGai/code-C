//
// Created by HP LAPTOP on 26/01/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct {
    char mssv[20];
    char tenbai[30];
    int diem;
    int max;
} LOG;

typedef struct {
    char tenbai[30];
    int maximum;
    int max;
    int min;
    int nop;
    int nop_maximum;
    double rate;
} BAI;

typedef struct {
    char tenbai[30];
    int submit_time;
    int max;
} BAINOP;

typedef struct {
    char mssv[20];
    int diem;
    BAINOP *bainop;
} SV;

int readloghs2(LOG** log) {
    int max_log = 1024;
    int cur_log = 0;
    while (true) {
        char* temp = (char*)malloc(1024);
        fgets(temp, 1023, stdin);
        if (temp[0] == '#') { break; }
        sscanf(temp, "%s %s %d %d",
            (*log)[cur_log].mssv,
            (*log)[cur_log].tenbai,
            &(*log)[cur_log].diem,
            &(*log)[cur_log].max
            );

        free(temp);
        temp = NULL;
        cur_log++;


        if (cur_log == max_log) {
            max_log <<= 1;
            LOG* tam = (LOG*)realloc(*log, max_log*sizeof(LOG));
            if (!tam) {
                fprintf(stderr, "Allocated unsuccessfully");
                exit(EXIT_FAILURE);
            }

            *log = tam;
        }
    }
    *log = (LOG*)realloc(*log, cur_log*sizeof(LOG));

    return cur_log;
}

int readbaihs2(BAI** bai, LOG* log, int dai_log) {
    int max_bai = 1024;
    int cur_bai = 0;

    for (int i = 0; i < dai_log; i++) {
        bool them = true;
        for (int j = 0; j < cur_bai; j++) {
            if (strcmp(log[i].tenbai, (*bai)[j].tenbai) == 0) {
                them = false;
                if (log[i].diem == (*bai)[j].maximum) {
                    (*bai)[j].nop_maximum++;
                    (*bai)[j].max = (*bai)[j].maximum;
                } else if (log[i].diem > (*bai)[j].max) {
                    (*bai)[j].max = log[i].diem;
                } else if (log[i].diem < (*bai)[j].min) {
                    (*bai)[j].min = log[i].diem;
                }
                (*bai)[j].nop++;
                break;
            }
        }
        if (them) {
            strcpy((*bai)[cur_bai].tenbai, log[i].tenbai);
            (*bai)[cur_bai].max = log[i].diem;
            (*bai)[cur_bai].min = log[i].diem;
            (*bai)[cur_bai].nop = 1;
            (*bai)[cur_bai].maximum = log[i].max;
            (*bai)[cur_bai].nop_maximum = 0;
            cur_bai++;
            if (cur_bai == max_bai) {
                max_bai <<= 1;
                *bai = (BAI*)realloc(*bai, max_bai*sizeof(BAI));
            }
        }
    }
    *bai = (BAI*)realloc(*bai, cur_bai*sizeof(BAI));
    for (int i = 0; i < cur_bai; i++) {
        (*bai)[i].rate = (*bai)[i].nop_maximum*1.0/(*bai)[i].nop;
    }

    return cur_bai;
}

void sortBai(BAI* bai, int bai_count) {
    for (int i = 0; i < bai_count; i++) {
        for (int j = i + 1; j < bai_count; j++) {
            if (strcmp(bai[i].tenbai, bai[j].tenbai) > 0) {
                BAI temp = bai[j];
                bai[j] = bai[i];
                bai[i] = temp;
            }
        }
    }
}

int readlsloghs2(char*** lslog, LOG* log, int dai_log) {
    int dai_lslog = 0;
    for (int i = 0; i < dai_log; i++) {
        bool them = true;
        for (int j = 0; j < dai_lslog; j++) {
            if (strcmp(log[i].mssv, (*lslog)[j]) == 0) {
                them = false;
                break;
            }
        }
        if (them) {
            (*lslog)[dai_lslog] = (char*)malloc(strlen(log[i].mssv) + 1);
            strcpy((*lslog)[dai_lslog], log[i].mssv);
            dai_lslog++;
        }
    }
    *lslog = (char**)realloc(*lslog, dai_lslog*sizeof(char*));

    return dai_lslog;
}

void sortSao(char** sao_lslog, int sv_count) {
    for (int i = 0; i < sv_count; i++) {
        for (int j = i + 1; j < sv_count; j++) {
            if (strcmp(sao_lslog[i], sao_lslog[j]) > 0) {
                char* temp = sao_lslog[j];
                sao_lslog[j] = sao_lslog[i];
                sao_lslog[i] = temp;
            }
        }
    }
}

void readsvhs2(SV* sv, int sv_count, LOG* log, int dai_log, int bai_count) {
    for (int i = 0; i < dai_log; i++) {
        for (int j = 0; j < sv_count; j++) {
            if (strcmp(log[i].mssv, sv[j].mssv) == 0) {
                for (int k = 0; k < bai_count; k++) {
                    if (strcmp(log[i].tenbai, sv[j].bainop[k].tenbai) == 0) {
                        sv[j].bainop[k].submit_time++;
                        if (log[i].diem > sv[j].bainop[k].max) {
                            sv[j].bainop[k].max = log[i].diem;
                        }
                        break;
                    }
                }
                break;
            }
        }
    }

    for (int i = 0; i < sv_count; i++) {
        for (int j = 0; j < bai_count; j++) {
            sv[i].diem += sv[i].bainop[j].max;
        }
    }
}


int hs2() {
    // lưu các log
    LOG *log = (LOG*)malloc(1024*sizeof(LOG));
    int dai_log = readloghs2(&log);


    // lưu các bài và sort theo ABC
    BAI *bai = (BAI*)malloc(1024*sizeof(BAI));
    int bai_count = readbaihs2(&bai, log, dai_log);
    sortBai(bai, bai_count);


    // lưu các lần nộp bài đầu tiên của hs
    char** lslog = (char**)malloc(dai_log*sizeof(char*));
    int sv_count = readlsloghs2(&lslog, log, dai_log);

    // tạo 1 bản sao và sắp xếp các sinh viên theo ABC
    char** sao_lslog = (char**)malloc(sv_count*sizeof(char*));
    for (int i = 0; i < sv_count; i++) {
        sao_lslog[i] = (char*)malloc(strlen(lslog[i]) + 1);
        strcpy(sao_lslog[i], lslog[i]);
    }
    sortSao(sao_lslog, sv_count);

    // lưu sinh viên
    SV* sv = (SV*)malloc(sv_count*sizeof(SV));
    for (int i = 0; i < sv_count; i++) {
        strcpy(sv[i].mssv, sao_lslog[i]);
        sv[i].bainop = (BAINOP*)malloc(bai_count*sizeof(BAINOP));
        sv[i].diem = 0;
        for (int j = 0; j < bai_count; j++) {
            strcpy(sv[i].bainop[j].tenbai, bai[j].tenbai);
            sv[i].bainop[j].max = 0;
            sv[i].bainop[j].submit_time = 0;
        }
    }
    readsvhs2(sv, sv_count, log, dai_log, bai_count);


    int yeucau;
    scanf("%d", &yeucau);
    if (yeucau == 1) {
        for (int i = 0; i < sv_count; i++) {
            printf("%s\n", lslog[i]);
        }
    } else if (yeucau == 2) {
        for (int i = 0; i < sv_count; i++) {
            printf("%s ", sv[i].mssv);
            for (int j = 0; j < bai_count; j++) {
                if (sv[i].bainop[j].submit_time > 0) {
                    printf("%s:%d ", sv[i].bainop[j].tenbai, sv[i].bainop[j].submit_time);
                }
            }
            printf("\n");
        }
    } else if (yeucau == 3) {
        for (int i = 0; i < sv_count; i++) {
            printf("%s %d\n", sv[i].mssv, sv[i].diem);
        }
    } else if (yeucau == 4) {
        char timbai[20];
        scanf("%s", timbai);
        for (int i = 0; i < bai_count; i++) {
            if (strcmp(timbai, bai[i].tenbai) == 0) {
                printf("%d %d %.2lf", bai[i].max, bai[i].min, bai[i].rate);
                break;
            }
        }
    }


    free(log);
    log = NULL;

    free(bai);
    bai = NULL;

    for (int i = 0; i < sv_count; i++) {
        free(lslog[i]);
        free(sao_lslog[i]);
        free(sv[i].bainop);
        lslog[i] = NULL;
        sao_lslog[i] = NULL;
        sv[i].bainop = NULL;
    }

    free(lslog);
    lslog = NULL;

    free(sao_lslog);
    sao_lslog = NULL;

    free(sv);
    sv = NULL;


    return 0;
}
