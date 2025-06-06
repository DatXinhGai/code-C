//
// Created by HP LAPTOP on 12/01/2025.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

char* readline2() {
    int data_len = 0;
    int alloc_len = 1024;

    char *data = malloc(alloc_len);

    while (true) {
        char* cursor = data + data_len;

        char* line = fgets(cursor, alloc_len - data_len, stdin);
        if (!line) { break; }

        data_len += strlen(line);

        if (data_len < alloc_len || data[data_len - 1] == '\n') { break; }

        alloc_len <<= 1;
        char* temp = realloc(data, alloc_len);
        if (!temp) { break; }
        data = temp;
    }

    data[strcspn(data, "\r\n")] = '\0';

    data = (char*)realloc(data, strlen(data) + 1);

    return data;
}

void chuanHoaXau2(char* xau) {
    char* det = xau;
    char* sour = xau;

    // tìm vị trí khác cách trống đầu tiên
    while (*sour == ' ') {
        sour++;
    }

    *det = *sour;
    sour++;
    det++;

    while (*sour != '\0') {
        if (*sour == ' ' && *(sour - 1) == ' ') {
            sour++;
        } else {
            *det = *sour;
            sour++;
            det++;
        }
    }
    if (*(det - 1) == ' ') {
        *(det - 1) = '\0';
    } else {
        *det = '\0';
    }
}

// dùng strrchr

char* inNguocstrrchr(char* xau) {
    char* ptr;
    char* sao = malloc(strlen(xau) + 1);
    char* ptrsao = sao;
    while (1) {
        ptr = strrchr(xau, ' ');        // tìm ' ' từ phải sang
        if (!ptr) {
            strcpy(ptrsao, xau);    // lần cuối không tìm được vẫn còn 1 từ
            break;
        }
        strcpy(ptrsao, ptr + 1);
        *(ptrsao + strlen(ptr) - 1) = ' ';  // thêm dấu cách
        ptrsao += strlen(ptr);              // ptrsao trỏ đến sau dấu cách
        *ptr = '\0';                        // rút ngắn str lại
    }

    return sao;
}


// LƯU Ý HÀNH VI CỦA strtok
/*
 * tìm các delimiter và cho bằng '\0'
 * tìm lại về vị trí đầu tiên và gán cho n_ptr
 * chuỗi mẹ bị thay đổi trực tiếp, từ phần sau vẫn giữ nguyên
 * lần cuối không tìm được delimiter thì trả về chuỗi chính là phần còn lại của chuỗi mẹ
 * YÊU CẦU CHUỖI PHẢI SỬA ĐƯỢC
 * nên nếu truyền vào hằng xâu như 'Hello World" thì sẽ gây lỗi
 */

char* inNguocstrtok(char* xau) {
    /*
    char* xausao = malloc(strlen(xau) + 1);
    strcpy(xausao, xau);
    */


    char* n_ptr = strtok(xau, " ");
    if (!n_ptr) {
        return xau;
    }

    int dai;
    char* sao = malloc(strlen(xau) + 1);
    strcpy(sao, n_ptr);
    while (1) {
        n_ptr = strtok(NULL, " ");
        if (!n_ptr) {
            break;
        }

        // memmove tạo khoảng trống cho chuỗi thêm vào và dấu cách
        // an toàn hơn memcpy nhưng chậm hơn
        dai = strlen(n_ptr);
        memmove(sao + dai + 1, sao, strlen(sao) + 1);
        *(sao + dai) = ' ';
        memcpy(sao, n_ptr, dai);
    }


    return sao;



    // không cần thiết nhưng kệ :))
    /*
    dai = strlen(xausao) - strlen(sao);
    memmove(sao + dai, sao, dai);
    sao[dai] = ' ';
    memcpy(sao, xausao + strlen(sao), dai + 1);

    free(xausao)
    */

}

// không cần chuẩn hóa
char* inNguocsscanf(char* xau) {

}



void inNguoc() {
    char* xau = readline2();
    chuanHoaXau2(xau);
    char* sao = inNguocstrtok(xau);
    puts(sao);



    free(xau);
    xau = NULL;
    free(sao);
    sao = NULL;

}


