//
// Created by HP LAPTOP on 08/01/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char stuID[11];
    char date[11];
    char time[9];
    int diem;
} Log;



typedef struct {
    char stuID[11];
    int diem;
    int nop;
} Stu;



void readlog(Log **log, int* log_len) {
    int max_len = 1024;
    *log = (Log*)malloc(max_len*sizeof(Log));
    do {
        char* temp = (char*)malloc(100);
        do {
            fgets(temp, 99, stdin);
            temp[strcspn(temp, "\r\n")] = '\0';
        } while (strlen(temp) == 0);
        if (temp[0] == '-') {
            free(temp);
            temp = NULL;
            break;
        }
        sscanf(temp, "%s %s %s %d", (*log)[*log_len].stuID, (*log)[*log_len].date, (*log)[*log_len].time, &((*log)[*log_len].diem));
        (*log_len)++;


        if (*log_len == max_len) {
            max_len <<= 1;
            Log* phu = (Log*)realloc(*log, max_len*sizeof(Log));
            if (!phu) {
                perror("Lỗi cấp phát");
                free(temp);
                temp = NULL;
                break;
            }
            *log = phu;
        }

    } while (1);
    *log = (Log*)realloc(*log, *log_len*sizeof(Log));
}




void readlenh(char*** lenh, int* lenh_len) {
    int max_len = 1024;
    *lenh = (char**)malloc(max_len*sizeof(char*));
    do {
        char* tam = (char*)malloc(100);
        do {
            fgets(tam, 99, stdin);
            tam[strcspn(tam, "\r\n")] ='\0';
        } while (strlen(tam) == 0);

        if (tam[0] == '-') {
            break;
        }

        (*lenh)[*lenh_len] = tam;
        (*lenh)[*lenh_len][strcspn((*lenh)[*lenh_len], "\r\n")] = '\0';

        (*lenh_len)++;

        if (*lenh_len == max_len) {
            max_len <<= 1;
            *lenh = (char**)realloc(*lenh, max_len*sizeof(char*));

        }
    } while (1);

    *lenh = (char**)realloc(*lenh, *lenh_len*sizeof(char*));
}



Stu* thongKeStu(int *soStu, int log_len, Log *log) {
    int maxStu = 1024;
    Stu* stu = (Stu*)malloc(maxStu*sizeof(Stu));
    strcpy(stu[0].stuID, log[0].stuID);
    stu[0].diem = log[0].diem;
    stu[0].nop = 1;
    *soStu = 1;
    for (int i = 0; i < log_len; i++) {
        int them = 1;
        for (int j = 0; j < *soStu; j++) {
            if (!strcmp(log[i].stuID, stu[j].stuID)) {
                stu[j].nop += 1;
                if (log[i].diem > stu[j].diem) {
                    stu[j].diem = log[i].diem;
                }
                them = 0;
                break;
            }
        }
        if (them) {
            strcpy(stu[*soStu].stuID, log[i].stuID);
            stu[*soStu].diem = log[i].diem;
            stu[*soStu].nop = 1;
            (*soStu)++;
            if (*soStu == maxStu) {
                maxStu <<= 1;
            }
            stu = (Stu*)realloc(stu, maxStu*sizeof(Stu));
            if (!stu) {
                perror("Lỗi cấp phát");
            }
        }
    }
    return stu;
}



void diemMinMax(int* min, int* max, Stu *stu, int soStu) {
    *min = stu[0].diem;
    *max = stu[0].diem;

    for (int i = 1; i < soStu; i++) {
        if (stu[i].diem < *min) {
            *min = stu[i].diem;
        } else if (stu[i].diem > *max) {
            *max = stu[i].diem;
        }
    }
}

int nopNhieuNhat(Stu *stu, int soStu) {
    int maxnop = stu[0].nop;
    for (int i = 1; i < soStu; i++) {
        if (stu[i].nop > maxnop) {
            maxnop = stu[i].nop;
        }
    }
    return maxnop;
}

void thongTinStu(Stu *stu, int soStu, char* stuID) {
    int nop;
    for (int i = 0; i < soStu; i++) {
        nop = 0;
        if (!strcmp(stu[i].stuID, stuID)) {
            nop = 1;
            puts("---------------------------");
            printf("Sinh viên %s có:\n", stu[i].stuID);
            printf("-Điểm: %d\n-Số lần nộp: %d\n", stu[i].diem, stu[i].nop);
            puts("---------------------------");
            break;
        }
    }
    if (!nop) {
        printf("Chưa nộp bài\n");
    }
}



void readlogStudent() {

    // lưu log
    int log_len = 0;
    Log *log;
    readlog(&log, &log_len);

    // lưu lệnh
    int lenh_len = 0;
    char** lenh;
    readlenh(&lenh, &lenh_len);

    // lưu sinh viên
    int soStu = 0;
    Stu* stu = thongKeStu(&soStu, log_len, log);


    for (int i = 0; i < lenh_len; i++) {
        if (!strcmp(lenh[i], "nopnhieu")) {
            int nhieu = nopNhieuNhat(stu, soStu);
            printf("Số lần nộp bài nhiều nhất là %d\n", nhieu);
        } else if (!strcmp(lenh[i], "minmax")) {
            int min, max;
            diemMinMax(&min, &max, stu, soStu);
            printf("Điểm thấp và cao nhất lần lượt là: %d %d\n", min, max);
        } else if (!strcmp(lenh[i], "nop")) {
            printf("Số sinh viên nộp bài là: %d", soStu);
        }
        else {
            thongTinStu(stu, soStu, lenh[i]);
        }
    }

    free(log);
    log = NULL;

    free(lenh);
    lenh = NULL;

}