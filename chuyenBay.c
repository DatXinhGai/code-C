//
// Created by HP LAPTOP on 04/02/2025.
//
/*
Ví dụ
INPUT
00:05;MANILA;VN647;T2
00:10;ZHENGZHOU, P. R. CHINA;GI4202;T2
00:10;BUSAN;VN428;T2
00:10;ULAN BATOR;M0536;T2
00:15;NAGOYA;VN348;T2
00:25;DUBAI (DXB);EK395;T2
00:25;TOKYO (NRT);NH898;T2
00:25;TOKYO (NRT);VN310;T2
00:35;HO CHI MINH;VJ163.;T1
00:40;LONDON (LHR);VN55;T2
00:50;TOKYO (NRT);VJ932;T2
#
1 VN348

OUTPUT
00:15 T2

INPUT
00:05;MANILA;VN647;T2
00:10;ZHENGZHOU, P. R. CHINA;GI4202;T2
00:10;BUSAN;VN428;T2
00:10;ULAN BATOR;M0536;T2
00:15;NAGOYA;VN348;T2
00:25;DUBAI (DXB);EK395;T2
00:25;TOKYO (NRT);NH898;T2
00:25;TOKYO (NRT);VN310;T2
00:35;HO CHI MINH;VJ163.;T1
00:40;LONDON (LHR);VN55;T2
00:50;TOKYO (NRT);VJ932;T2
#
2 00:10 TOKYO

OUTPUT
00:25 TOKYO (NRT) NH898 T2
00:25 TOKYO (NRT) VN310 T2
00:50 TOKYO (NRT) VJ932 T2
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    char* tg;
    char* ten;
    char* chuyen;
    char* ga;
} THONGTIN;

typedef struct {
    THONGTIN* chuyen;
    int chuyen_count;
    int chuyen_max;
    char* ten;
} TEN;
char* readlinecB(void);
char* ltrimcB(char* data);
char* rtrimcB(char* data);
char** split_stringcB(char* data);

int chuyenBay() {
    int dem_tt = 0;
    int max_tt = 1024;
    THONGTIN *tt = (THONGTIN*)malloc(max_tt*sizeof(THONGTIN));

    int dem_ten = 0;
    int max_ten = 1024;
    TEN* name = (TEN*)malloc(max_ten*sizeof(TEN));

    while (true) {
        char* data = readlinecB();
        if (data[0] == '#') { break; }
        char** du_lieu = split_stringcB(ltrimcB(rtrimcB(data)));
        tt[dem_tt].tg = du_lieu[0];
        tt[dem_tt].ten = du_lieu[1];
        tt[dem_tt].chuyen = du_lieu[2];
        tt[dem_tt].ga = du_lieu[3];

        bool them_ten = true;
        for (int i = 0; i < dem_ten; i++) {
            if (strstr(tt[dem_tt].ten, name[i].ten)) {
                them_ten = false;
                name[i].chuyen[name[i].chuyen_count] = tt[dem_tt];
                name[i].chuyen_count++;
                if (name[i].chuyen_count >= name[i].chuyen_max) {
                    name[i].chuyen_max <<= 1;
                    name[i].chuyen = (THONGTIN*)realloc(name[i].chuyen, name[i].chuyen_max*sizeof(THONGTIN));
                }
                break;
            }
        }

        if (them_ten) {
            name[dem_ten].chuyen_count = 1;
            name[dem_ten].chuyen_max = 32;
            name[dem_ten].ten = tt[dem_tt].ten;
            name[dem_ten].chuyen = (THONGTIN*)malloc(name[dem_ten].chuyen_max*sizeof(THONGTIN));
            name[dem_ten].chuyen[0] = tt[dem_tt];
            dem_ten++;
        }

        dem_tt++;
        if (dem_tt >= max_tt) {
            max_tt <<= 1;
            tt = (THONGTIN*)realloc(tt, max_tt*sizeof(THONGTIN));
        }
        data = NULL;
        for (int i = 0; i < 4; i++) {
            du_lieu[i] = NULL;
        }
        du_lieu = NULL;
    }
    tt = (THONGTIN*)realloc(tt, dem_tt*sizeof(THONGTIN));


    int query;
    scanf("%d", &query);
    if (query == 1) {
        char chuyen[10];
        scanf("%s", chuyen);
        int i;
        bool hople = false;
        for (i = 0; i < dem_tt; i++) {
            if (strstr(tt[i].chuyen, chuyen)) {
                hople = true;
                printf("%s %s\n", tt[i].tg, tt[i].ga);
                break;
            }
        }
        if (!hople) {
            puts("-1");
        }


    } else {
        char tg[6];
        char *ten = (char*)malloc(128);
        scanf("%s", tg);
        fgets(ten, 127, stdin);
        ten[strcspn(ten, "\r\n")] = '\0';
        ten = ltrimcB(rtrimcB(ten));
        int tim = false;
        for (int i = 0; i < dem_tt; i++) {
            if (strstr(tt[i].ten, ten)) {
                if (strcmp(tt[i].tg, tg) >= 0) {
                    tim = true;
                    printf("%s %s %s %s\n",
                        tt[i].tg,
                        tt[i].ten,
                        tt[i].chuyen,
                        tt[i].ga);
                }
            }
        }
        if (!tim) {
            puts("-1");
        }
    }

    free(tt);
    tt = NULL;
    for (int i = 0; i < dem_ten; i++) {
        name[i].chuyen = NULL;
        // free tt rồi mà free tiếp là giải phóng vùng nhớ đã giải phóng
    }
    free(name);
    name = NULL;
    return 0;
}

char* readlinecB(void) {
    int data_len = 0;
    int alloc_len = 1024;
    char* data = (char*)malloc(alloc_len);

    while (true) {
        char* cursor = data + data_len;
        char* line = fgets(cursor, alloc_len - data_len, stdin);
        if (!line) { break; }

        data_len += strlen(line);
        if (data_len < alloc_len || data[data_len - 1] == '\n') {
            break;
        }

        alloc_len <<= 1;
        char* temp = (char*)realloc(data, alloc_len);
        if (!temp) { break; }
        data = temp;
    }
    if (data[data_len - 1] == '\n') {
        data_len--;
    }
    data[data_len] = '\0';

    data = (char*)realloc(data, data_len + 1);
    return data;
}

char* ltrimcB(char* data) {
    char* str = data;
    if (!data) {
        return NULL;
    }
    if (!*data) {
        return "\0";
    }
    while (*data == ' ') {
        data++;
    }
    str = data;
    data = NULL;
    return str;
}

char* rtrimcB(char* data) {
    char* ptr = data + strlen(data) - 1;
    while (*ptr == ' ') {
        ptr--;
    }
    ptr++;
    ptr = '\0';
    return data;
}

char** split_stringcB(char* data) {
    char** split = (char**)malloc(4*sizeof(char*));
    // c1 là dùng strtok gặp ';' thì cắt chuỗi


    int dem = 0;
    char* token = strtok(data, ";");
    while (token != NULL) {
        split[dem++] = token;
        token = strtok(NULL, ";");
    }

    return split;

    // c2 là dùng con trỏ n_ptr, gặp ';' thì đổi thành '/0' chơi XOR cho máu
}