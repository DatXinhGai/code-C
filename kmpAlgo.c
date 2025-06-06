//
// Created by HP LAPTOP on 11/02/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
in
axxxxyyyyb
xy

out
ab


in
daabcbaabcbc
abc

out
dab
*/


int* precomputeLPS(char* part) {
    int* LPS = (int*)malloc(strlen(part)*sizeof(int));
    LPS[0] = 0;
    int len = 0;
    int i = 1;
    while (i < strlen(part)) {
        if (part[i] == part[len]) {
            len++;
            LPS[i] = len;
            i++;
        } else {
            if (len > 0) {
                len = LPS[len - 1]; // quay về LPS trước đó
            } else {
                LPS[i] = 0;
                i++;
            }
        }
    }
    return LPS;
}

char* removeOccurrence(char* s, char* part, int* LPS) {
    int s_len = strlen(s);
    int part_len = strlen(part);
    if (s_len < part_len) {
        return s;
    }
    int* mis = (int*)malloc(s_len*sizeof(int));
    // mảng mis để truy xuất xem phần tiếp theo có thể chạy sau khi loại được chuỗi con
    mis[0] = 0;
    char* n_ptr = s;
    char* end_ptr = s;
    int len = 0;
    int* mis_ptr = mis;

    while (*n_ptr) {
        *end_ptr = *n_ptr;
        end_ptr++;

        if (*(end_ptr - 1) == part[len]) {  // bằng nhau
            len++;
        } else {                            // không giống
            // tìm phần mà prefix giống sẵn rồi
            while (len > 0 && *(end_ptr - 1) != part[len]) {
                len = LPS[len - 1];
            }

            // nếu tìm được prefix
            if (*(end_ptr - 1) == part[len]) {
                len++;
            }
        }

        *mis_ptr = len;
        mis_ptr++;

        if (len == part_len) {
            end_ptr -= part_len;
            mis_ptr -= part_len;
            if (mis_ptr > mis) {
                len = *(mis_ptr - 1);
            } else {
                len = 0;
            }
        }
        n_ptr++;
    }
    *end_ptr = '\0';
    free(mis);
    mis = NULL;

    return s;
}

int kmpAlgo() {
    char* s = (char*)malloc(1024);
    char* part = (char*)malloc(1024);
    fgets(s, 1023, stdin);
    s[strcspn(s, "\r\n")] = '\0';
    fgets(part, 1023, stdin);
    part[strcspn(part, "\r\n")] = '\0';



    int* LPS = precomputeLPS(part);
    char* re = removeOccurrence(s, part, LPS);
    puts(re);

    free(s);
    s = NULL;
    free(part);
    part = NULL;

    return 0;
}