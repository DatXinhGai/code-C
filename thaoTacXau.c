//
// Created by HP LAPTOP on 04/01/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char* readline() {
    int data_length = 0;
    int alloc_length = 1024;

    char* data = (char*)malloc(alloc_length);
    if (!data) { return 0; }

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);    // ghi thẳng vào data
        if (!line) { break; }

        data_length += strlen(line);
        if (data_length != alloc_length || data[data_length - 1] == '\n') {
            break;
        }    // kết thúc nhập

        int new_length = alloc_length << 1;
        char *temp = (char*)realloc(data, new_length);
        if (!temp) { break; }

        data = temp;
        alloc_length = new_length;
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
        data_length--;
    }

    char* temp = (char*)realloc(data, data_length);
    if (temp) {
        data = temp;
    }

    return data;
}




// hơi dài
/*
char* ltrim(char** data) {
    int data_length = strlen(*data);
    int trai = 0;
    while ((*data)[trai] == ' ') { trai++; }

    data_length -= trai;
    char* temp = (char*)realloc(*data, data_length);
    if (!temp) { return 0;}

    *data = temp;
    return *data;
}
*/

char* ltrim(char* str) {
    // char* traVe = (char*)realloc(str + dich, strlen(str) - dich + 1);
    // gây ra lỗi vì vùng str + dich chưa được cấp phát;
    // tốt nhất là malloc thằng mới rồi strcpy
    // còn không thì memcpy cũng được



    /*
        if (!str) {
            return '\0';
        }

        if (!*str) {
            return str;
        }

        while (*str != '\0' && isspace(*str)) {
            str++;
        }

        return str;
    } */
    // cũng lỗi vì dịch str rủi ro cao





    int dich = 0;
    while (str[dich] == ' ' && str[dich] != '\0') {
        dich++;
    }


    /*
    char* traVe = (char*)malloc(strlen(str) - dich + 1);
    strcpy(traVe, str + dich);
    free(str);
    str = NULL;
    return traVe;
    */


    memcpy(str, str + dich, strlen(str) - dich + 1);
    // đúng đủ và nhanh hơn memmove trong trường hợp này khi không quan tâm dữ liệu thừa
    return str;

}


char* rtrim(char* str) {
    if (!str) { return 0; }

    if (!*str) { return str; }

    char* end = str + strlen(str) - 1;
    while (*end == ' ') {
        end--;
    }

    *(end + 1) = '\0';   // lưu ý

    return str;
}

char** split_string(char* str) {
    int spaces = 1;
    char** splits = (char**)malloc(sizeof(char*));
    char* token = strtok(str, " ");

    while (token) {
        char** temp = (char**)realloc(splits, sizeof(char*) * spaces++);
        if (!temp) { return splits; }
        splits = temp;

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }
    char** temp = (char**)realloc(splits, sizeof(char*) * --spaces);
    if (temp) { splits = temp; }

    return splits;
}


int parse_int(char* str) {
    char* endptr;
    int num;

    num = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != 0) {
        fprintf(stderr, "Can't parse int!");
        exit(EXIT_FAILURE);
    }

    return num;
}

// mất bộ nhớ cho mảng phụ
char* chuanHoaMangPhu(char* str) {
    int dai = strlen(str);
    char* phu = (char*)malloc(dai);
    int dem = 0;
    int chiSo = 0;
    for (int i = 0; i < dai; i++) {
        if (str[i] != ' ') {
            break;
        }
        dem++;
    }
    for (int i = dem; i <= dai; i++) {
        if (str[i] != ' ' || (str[i] == ' ' && str[i - 1] != ' ')) {
            phu[chiSo++] = str[i];
        }
    }
    if (phu[chiSo - 1] == ' ') {
        phu[chiSo - 1] = '\0';
        chiSo--;
    }
    phu = (char*)realloc(phu, chiSo);

    free(str);
    str = NULL;
    return phu;
}

// nếu nhiều dấu cách lẻ tẻ thì không tối ưu
char* chuanHoa_strcpy(char* str) {
    char* chay = str;
    while (*chay != '\0') {
        if (*chay == ' ') {
            char* luu = chay++;

            while (*chay  == ' ' && *chay != '\0') {
                chay++;
            }
            if (chay - luu > 1) {
                strcpy(luu + 1, chay);
                chay = luu + 1;
            }
        } else {
            chay++;
        }
    }
    if (*str == ' ') {
        strcpy(str, str + 1);
    }
    if (*(chay - 1) == ' ') {
        *(chay - 1) = '\0';
    }
    return str;

}

// tối ưu về cả thời gian và bộ nhớ
char* chuanHoa_contro(char* str) {
    char* det = str;
    char* sour = str;
    while (*det == ' ' && *det != '\0') {
        det++;
    }
    *(sour++) = *(det++);
    while (*det != '\0') {
        if (*det == ' ') {
            *(sour++)= *(det++);
            while (*det == ' ' && *det != '\0') {
                det++;
            }
        } else {
            *(sour++) = *(det++);
        }
    }
    if (*(sour - 1) == ' ') {
        *(sour - 1) = '\0';
    } else {
        *(sour) = '\0';
    }
    return str;
}

// viết cho strtok
char** tokenize_contro(char* xau, int* dai_mang) {
    int max_mang = 1024;

    char* moc = xau;
    char* chay = xau;
    char** mangtok = (char**)malloc(max_mang*sizeof(char*));

    while (*chay != '\0') {
        if (*chay != ' ') {
            chay++;
        } else {
            *chay = '\0';
            mangtok[*dai_mang] = (char*)malloc(chay - moc + 1);
            strcpy(mangtok[(*dai_mang)++], moc);

            if (*dai_mang == max_mang) {
                max_mang <<= 1;
                mangtok = (char**)realloc(mangtok, max_mang*sizeof(char*));
            }

            moc = (chay++) + 1;
        }
    }
    mangtok[*dai_mang] = (char*)malloc(chay - moc + 1);
    strcpy(mangtok[(*dai_mang)++], moc);

    mangtok = (char**)realloc(mangtok, *dai_mang*sizeof(char*));

    return mangtok;
}

//char** tokenize_

void token() {
    char* s = chuanHoa_strcpy(readline());
    int dai_mang = 0;
    char** cat = tokenize_contro(s, &dai_mang);
    for (int i = 0; i < dai_mang; i++) {
        puts(cat[i]);
    }

}



void chuanHoa() {
    char* s = readline();
    s = chuanHoa_contro(s);
    puts(s);
}


void chuoiTangMax() {
    // phải luôn kiểm tra tìm file
    //char *path = getenv("OUTPUT_PATH");

    printf("Đang mở file...\n");
    _sleep(1000);
    char* file = "output.txt";
    FILE* fptr = fopen(file, "w");
    if (!fptr) {
        printf("Không thành công\n");
        _sleep(1000);
        exit(EXIT_FAILURE);
    } else {
        printf("Mở file thành công!\n");
    }
    
    _sleep(1000);
    printf("Nhập các số và xuống dòng để kết thúc\n");

    // char** day_temp = split_string(rtrim(ltrim(readline())));
    // nếu thế này thì không thể tìm số phần tử

    char* day_temp = ltrim(rtrim(readline()));
    char* nptr = day_temp;
    char* endptr;

    int max_len = 1024;
    int cur_len = 0;
    int* day = (int*)malloc(max_len*sizeof(int));

    do {
        while (*nptr == ' ' || *nptr == '\t') {
            nptr++;
        }
        day[cur_len++] = strtol(nptr, &endptr, 10);
        if (nptr == endptr) { break; }
        nptr = endptr;

        if (cur_len == max_len) {
            max_len <<= 1;
            int* temp = (int*)realloc(day, max_len*sizeof(int));
            if (!temp) {
                fprintf(stderr, "Not enough spaces for day!");
                exit(EXIT_FAILURE);
            }

            day = temp;
        }
    } while (*nptr != '\0');

    day = (int*)realloc(day, (cur_len + 1) * sizeof(int));


    int* phu = (int*)malloc(cur_len*sizeof(int));
    phu[0] = 1;
    int chiSo = 0;
    int max = 0;

    for (int i = 1; i < cur_len; i++) {
        if (day[i] > day[i - 1]) {
            phu[i] = phu[i - 1] + 1;
            if (phu[i] > max) {
                max = phu[i];
                chiSo = i;
            }
        } else {
            phu[i] = 1;
        }
    }


    fprintf(fptr, "Độ dài chuỗi tăng dài nhất là: %d\n", max);
    fprintf(fptr, "Bắt đầu từ phần tử thứ: %d\n", chiSo - max + 1);
    fprintf(fptr, "Các phần tử lần lượt là: \n");
    for (int i = chiSo - max + 1; i <= chiSo; i++) {
        fprintf(fptr, "%d ", day[i]);
    }

    _sleep(1000);
    printf("Kết quả đã được ghi vào file %s\n", file);
    _sleep(1000);
    printf("Đang đóng file và kết thúc!\n");
    _sleep(1000);
    printf("Exit in: \n");
    for (int i = 3; i > 0; i--) {
        _sleep(1000);
        printf("%d\n", i);
    }
    _sleep(1000);

    // nhớ phải close
    fclose(fptr);

    free(day_temp);
    day_temp = NULL;
    free(day);
    day = NULL;
    free(phu);
    phu = NULL;
}
