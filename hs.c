//
// Created by HP LAPTOP on 09/01/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "thaoTacXau.h"

// không được gán giá trị khi khai báo struct
typedef struct {
    char svID[11];
    char btID[50];
    char ngay[11];
    char tg[10];
    int diem;
    int thuTu;      // lưu thứ tự xuất hiện của các log để tí sắp xếp
} LOG;

typedef struct {
    char svID[11];
    int diem;
    int bainop;
    LOG **nop;
    // dùng nop cũng được nhưng mất thời gian gán và tốn bộ nhớ, thay vì vậy dùng mảng con trỏ trỏ đến LOG;
    // bản chất là con trỏ trỏ đến con trỏ dạng LOG, chứa nhiều phần tử kiểu LOG
} SV;

typedef struct {
    char btID[50];
    double tb;
    int bainop;
} Question;

char* readLine() {
    int data_len = 0;
    int alloc_len = 1024;
    char* data = (char*)malloc(alloc_len);

    while (true) {
        char* cursor = data + data_len;

        char* line = fgets(cursor, alloc_len - data_len, stdin);
        if (!line) {
            perror("Allocated unsuccessfully");
        }

        data_len += strlen(line);
        if (data_len < alloc_len || data[data_len - 1] == '\n') {
            break;
        }

        alloc_len <<= 1;
        char* temp = (char*)realloc(data, alloc_len);
        if (!temp) {
            perror("Allocated unsuccessfully");
            exit(EXIT_FAILURE);
        }
        data = temp;
        temp = NULL;
    }
    data[strcspn(data, "\r\n")] = '\0';

    data = (char*)realloc(data,strlen(data) + 1);
    return data;
}


// mảng lưu log và số log
void readLog(LOG **log, int *log_line) {
    int max_line = 1024;
    *log_line = 0;
    while (1) {
        char* temp = readLine();
        if (temp[0] == '-') {
            free(temp);
            temp = NULL;
            break;
        }
        sscanf(temp,
            "%s %s %s %s %d",
            (*log)[*log_line].svID,
            (*log)[*log_line].btID,
            (*log)[*log_line].ngay,
            (*log)[*log_line].tg,
            &(*log)[*log_line].diem);

        (*log)[*log_line].thuTu = *log_line;
        (*log_line)++;
        if (*log_line == max_line) {
            max_line <<= 1;
            *log = (LOG*)realloc(*log, max_line*sizeof(LOG));
        }
        temp = NULL;
    }
    *log = (LOG*)realloc(*log, *log_line*sizeof(LOG));
}


// mảng lưu câu hỏi và số câu hỏi
void readQuestion(Question** question, int* questionNum) {
    int max_num = 1024;
    *questionNum = 0;
    while (1) {

        char* temp = rtrim(readLine());
        if (temp[0] == '-') {
            free(temp);
            temp = NULL;
            break;
        }
        strcpy((*question)[*questionNum].btID, temp);
        (*questionNum)++;

        if (*questionNum == max_num) {
            max_num <<= 1;
            Question* tam = (Question*)realloc(*question, max_num*sizeof(Question));
            if (!tam) {
                perror("Cấp phát không thành công");
            }
            *question = tam;
        }
    }

    *question = (Question*)realloc(*question, *questionNum*sizeof(Question));
}





// thống kê tất cả mọi thứ về sinh viên

void demSv(LOG* log, int log_line, SV **sv, int* svNum, int questionNum) {
    int max_sv = 1024;
    *svNum = 1;
    strcpy((*sv)[0].svID, log[0].svID);
    (*sv)[0].nop = (LOG**)malloc(questionNum*sizeof(LOG*));
    (*sv)[0].nop[0] = &log[0];     //*((*sv)[0].nop + (*sv)[0].bainop) = log[0];
    (*sv)[0].bainop = 1;
    for (int i = 1; i < log_line; i++) {
        if (!strcmp(log[i].svID, (*sv)[0].svID)) {
            bool them = true;
            for (int j = 0; j < (*sv)[0].bainop; j++) {
                if (strstr((*sv)[0].nop[j]->btID, log[i].btID)) {
                    them = false;
                    if (log[i].diem > (*sv)[0].nop[j]->diem) {
                        (*sv)[0].nop[j] = &log[i];
                    }
                    break;
                }
            }
            if (them) {
                (*sv)[0].nop[(*sv)[0].bainop] =  &log[i];
                (*sv)[0].bainop++;
            }
        }
    }

    for (int i = 1; i < log_line; i++) {
        bool them_hs = true;

        for (int j = 0; j < *svNum; j++) {
            if (!strcmp((*sv)[j].svID, log[i].svID)) {      // so sánh các học sinh
                them_hs = false;    // cùng 1 học sinh
                bool them_bai = true;   // xem có nộp bài mới không

                for (int k = 0; k < (*sv)[j].bainop; k++) {
                    if (!strcmp((*sv)[j].nop[k]->btID, log[i].btID)) {   // so sánh các bài
                        them_bai = false;
                        if ((*sv)[j].nop[k]->diem < log[i].diem) {   // 2 bài giống nhau thì lấy điểm cao hơn
                            (*sv)[j].nop[k] = &log[i];
                        }
                        break;
                    }
                }
                if (them_bai) {
                    (*sv)[j].nop[(*sv)[j].bainop] = &log[i];
                    (*sv)[j].bainop++;
                }
            }
        }

        if (them_hs) {
            strcpy((*sv)[*svNum].svID, log[i].svID);
            (*sv)[*svNum].nop = (LOG**)malloc(questionNum*sizeof(LOG*));
            (*sv)[*svNum].nop[0] = &log[i];
            (*sv)[*svNum].bainop = 1;
            (*svNum)++;
            if (*svNum == max_sv) {
                max_sv <<= 1;
                *sv = (SV*)realloc(*sv,max_sv*sizeof(SV));
            }
        }
    }
    for (int i = 0; i < *svNum; i++) {
        (*sv)[i].diem = 0;
    }
    for (int i = 0; i < *svNum; i++) {
        (*sv)[i].nop = (LOG**)realloc((*sv)[i].nop, (*sv)[i].bainop*sizeof(LOG*));
        for (int j = 0; j < (*sv)[i].bainop; j++) {
            (*sv)[i].diem += (*sv)[i].nop[j]->diem;
        }
    }

    *sv = (SV*)realloc(*sv, *svNum*sizeof(SV));
}



void xapXepNop(const SV* sv, int svNum) {
    for (int i = 0; i < svNum; i++) {
        for (int j = 0; j < sv[i].bainop; j++) {
            for (int k = j + 1; k < sv[i].bainop; k++) {
                if (sv[i].nop[k]->thuTu < sv[i].nop[j]->thuTu) {
                    LOG* temp = sv[i].nop[k];
                    sv[i].nop[k] = sv[i].nop[j];
                    sv[i].nop[j] = temp;
                }
            }
        }
    }
}


// tính điểm trung bình và lần nộp của sinh viên
void tinhTrungBinh(Question* question, int questionNum, const LOG* log, int log_line) {
    for (int j = 0; j < questionNum; j++) {
        question[j].tb = 0;
        question[j].bainop = 0;
    }
    for (int i = 0; i < log_line; i++) {
        for (int j = 0; j < questionNum; j++) {
            if (!strcmp(question[j].btID, log[i].btID)) {
                question[j].tb += log[i].diem;
                question[j].bainop++;
            }
        }
    }
    for (int j = 0; j < questionNum; j++) {
        if (question[j].bainop != 0) {
            question[j].tb /= question[j].bainop;
        }
    }
}





// hàm in số sinh viên submit
void inSubmit(int svNum) {
    printf("Number of submits: %d\n", svNum);
}



void inDiem(SV sv) {
    printf("%s\n%d\n", sv.svID, sv.diem);
}


// hàm sắp xếp thứ tự nộp bài của sinh viên theo thời gian và in ra
void inChiTietSv(SV sv) {
    printf("%s\n", sv.svID);
    for (int i = 0; i < sv.bainop; i++) {
        printf("%s %s %s %d\n", sv.nop[i]->btID, sv.nop[i]->ngay, sv.nop[i]->tg, sv.nop[i]->diem);
    }
}



void inQuestion(Question* question, int questionNum) {
    for (int i = 0; i < questionNum; i++) {
        printf("%s %d %.2lf\n", question[i].btID, question[i].bainop, question[i].tb);
    }
}



void getSvInfoFromLog() {
    int dem = 0;
    int log_line = 0;
    LOG* log = (LOG*)malloc(1024*sizeof(LOG));  // PHẢI CẤP PHÁT TRƯỚC KHI LẤY ĐỊA CHỈ CHO VÀO HÀM!!!

    int questionNum = 0;
    Question* question = (Question*)malloc(1024*sizeof(Question));


    int svNum = 0;
    SV *sv = (SV*)malloc(1024*sizeof(SV));

    char* tam = malloc(100);
    char* lenh;
    while (dem < 3) {
        do {
            lenh = rtrim(readLine());
            lenh[strcspn(lenh, "\r\n")] = '\0';
        } while (strlen(lenh) == 0);
        if (lenh[0] == '-') {
            dem += 1;
        } else if (lenh[0] == '?') {
            if (strstr(lenh, "loadLogSubmit")) {
                readLog(&log, &log_line);
                dem++;
                printf("Number of logs: %d\n", log_line);
                demSv(log, log_line, &sv, &svNum, questionNum);
                xapXepNop(sv, svNum);
                tinhTrungBinh(question, questionNum, log, log_line);
            } else if (strstr(lenh, "loadExamQuestions")) {
                readQuestion(&question, &questionNum);
                dem++;
                printf("Number of questions: %d\n", questionNum);
            } else if (strstr(lenh, "numberofSubmits")) {
                inSubmit(svNum);
            } else if (strstr(lenh, "getSubmitStatistic")) {
                inQuestion(question, questionNum);
            } else if (strstr(lenh, "getStudentResults")) {
                tam = (char*)realloc(tam, strlen(lenh) + 1);
                strcpy(tam, "getStudentResults");
            } else {
                tam = (char*)realloc(tam, strlen(lenh) + 1);
                strcpy(tam, "getStudentSubmitDetail");
            }
        } else {
            int nop = 0;
            if (strstr(tam, "getStudentResults")) {
                for (int i = 0; i < svNum; i++) {
                    if (strstr(lenh, sv[i].svID)) {
                        inDiem(sv[i]);
                        nop = 1;
                        break;
                    }
                }
            } else {
                for (int i = 0; i < svNum; i++) {
                    if (strstr(lenh, sv[i].svID)) {
                        inChiTietSv(sv[i]);
                        nop = 1;
                        break;
                    }
                }
            }
            if (!nop) {
                printf("Sinh viên chưa nộp bai");
            }
        }
    }
    free(log);
    log = NULL;
    free(question);
    question = NULL;
    free(sv);
    sv = NULL;
    free(tam);
    tam = NULL;


}



// mảng lưu sinh viên và số sinh viên
/*
void demSv(LOG* log, int log_line, SV **sv, int* svNum, int questionNum) {
    int max_sv = 1024;
    *svNum = 1;
    *sv = (SV*)malloc(max_sv*sizeof(SV));

    strcpy((*sv)[0].svID, log[0].svID);
    (*sv)[0].nop = (LOG*)malloc(questionNum*sizeof(LOG));
    (*sv)[0].nop[0] = log[0];
    (*sv)[0].bainop = 1;

    for (int i = 1; i < log_line; i++) {
        bool them1 = true;                  // thêm học sinh mới
        for (int j = 0; j < *svNum; j++) {
            if (!strcmp((*sv)[j].svID, log[i].svID)) {
                them1 = false;
                bool them2 = true;          // thêm bài chưa nộp của học sinh cũ
                for (int k = 0; k < (*sv)[j].bainop; k++) {
                    if (!strcmp((*sv)[j].nop[k].btID, log[i].btID)) {
                        them2 = false;
                        if (log[i].diem > (*sv)[j].nop[k].diem) {
                            (*sv)[j].nop[k] = log[i];
                        }
                        break;
                    }
                }
                if (them2) {    // thêm bài chưa nộp
                    (*sv)[j].nop[(*sv)[j].bainop++] = log[i];
                }
                break;
            }
        }
            if (them1) {    // thêm học sinh mới
                strcpy((*sv)[*svNum].svID, log[i].svID);
                (*sv)[*svNum].nop[0] = log[i];
                (*sv)[*svNum].bainop = 1;
                (*svNum)++;
            }
    }

    for (int i = 0; i < *svNum; i++) {
        (*sv)[*svNum].nop = (LOG*)realloc((*sv)[*svNum].nop, (*sv)[*svNum].bainop);
    }
    *sv = (SV*)realloc(*sv, *svNum*sizeof(SV));
}
*/