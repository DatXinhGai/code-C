//
// Created by HP LAPTOP on 10/01/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* readline1() {

    int alloc_len = 1024;
    int data_len = 0;
    char* data = (char*)malloc(alloc_len);
    while (1) {
        char* cursor = data + data_len;

        char* line = fgets(cursor, alloc_len - data_len, stdin);
        if (!line) {
            perror("Không thể cấp phát");
            break;
        }

        data_len += strlen(line);

        if (data_len < alloc_len || data[data_len - 1] == '\n') {
            break;
        }

        alloc_len <<= 1;
        char* temp = (char*)realloc(data, alloc_len);
        if (!temp) {
            break;
        }
        data = temp;
    }
    data[strcspn(data, "\r\n")] = '\0';

    data = (char*)realloc(data, strlen(data) + 1);

    return data;
}

// c1 dùng strstr dịch từ trái sang
void strstrCon(const char* me, const char* con, int* mang, int *dem) {

    char* n_ptr = me;
    int max_mang = 1024;
    do {
        n_ptr = strstr(n_ptr, con);
        if (n_ptr == NULL || *n_ptr == '\0') {
            break;
        }
        mang[*dem] = n_ptr - me;
        n_ptr += strlen(con);
        (*dem)++;
        if (*dem == max_mang) {
            max_mang <<= 1;
            mang = (int*)realloc(mang, max_mang*sizeof(int));
        }
    } while (1);

    mang =(int*)realloc(mang, *dem*sizeof(int));

}

// c2 dùng quy hoạch động tìm chuỗi con chung dài nhất
void QHDxuatHien(const char* me, const char* con, int* mang, int *dem) {
    int daime = strlen(me);
    int daicon = strlen(con);
    if (daicon == 1) {
        for (int i = 0; i < daime; i++) {
            if (me[i] == con[0]) {
                mang[*dem] = i;
                (*dem)++;
            }
        }
    }
    else {
        int **bang = (int**)calloc(daime,sizeof(int*));
        for (int i = 0; i < daime; i++) {
            *(bang + i) = (int*)calloc(daicon,sizeof(int));
        }
        for (int i = 0; i < daime; i++) {
            if (me[i] == con[0]) {
                bang[i][0] = 1;
            }
        }
        for (int j = 1; j < daicon; j++) {
            if (me[0] == con[j]) {
                bang[0][j] = 1;
            }
        }
        for (int i = 1; i < daime; i++) {
            for (int j = 1; j < daicon; j++) {
                if (me[i] == con[j]) {
                    bang[i][j] = bang[i - 1][j - 1] + 1;
                    if (bang[i][j] == daicon) {
                        mang[*dem] = i;
                        (*dem)++;
                    }
                }
            }
        }
        for (int i = 0; i < daime; i++) {
            free(bang[i]);
            bang[i] = NULL;
        }
        free(bang);
        bang = NULL;
    }
}

void timViTriChuoiCon() {
    char* me;
    char* con;
    do {
        me = readline1();
        con = readline1();
        me[strcspn(me, "\r\n")] = '\0';
        con[strcspn(con, "\r\n")] = '\0';
    } while (strlen(me) == 0 || strlen(con) == 0);
    int dem = 0;
    int* mang = (int*)malloc(1024*sizeof(int));



    //strstrCon(me, con, mang, &dem);
    QHDxuatHien(me, con, mang, &dem);
    printf("Số lần xuất hiện: %d\n", dem);
    for (int i = 0; i < dem; i++) {
        printf("%d ", mang[i]);
    }
    free(mang);
    mang = NULL;
}
