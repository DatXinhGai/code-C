//
// Created by HP LAPTOP on 10/02/2025.
//
// stack vào cuộc
// thằng nào không trùng thằng sau thì push vào stack, trùng thì pop khỏi stack
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
char* readline123() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = (char*)malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = (char*)realloc(data, alloc_length);

        if (!data) {


            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = (char*)realloc(data, data_length);

        if (!data) {

        }
    } else {
        data = (char*)realloc(data, data_length + 1);

        if (!data) {

        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}



char* reduce(char* s) {
    if (strlen(s) <= 1) {
        return s;
    }
    char* n_ptr = s + 1;
    char* end_ptr = s;
    while (true) {
        if (*n_ptr == *end_ptr) {
            if (end_ptr == s) {
                *end_ptr = *(n_ptr + 1);
                n_ptr++;
                if (*n_ptr == '\0') {
                    break;
                }
            } else {
                end_ptr--;
            }
        } else {
            end_ptr++;
            *end_ptr = *n_ptr;
        }
        n_ptr++;
        if (*n_ptr == '\0') {
            break;
        }
    }
    *(end_ptr + 1) = '\0';
    if (*s == '\0') {
        return strdup("Empty String");
        // s = strdup("Empty String") là sai
        // do s chỉ là con trỏ sao của s trong hàm main
        // do đó chỉ thay đổi giá trị của s trong reduce
        // còn s trong hàm main bị thay đổi bởi con trỏ sao cùng trỏ đến s
        // muốn thay đổi phải truyền vào địa chỉ của s
    }
    return s;
}

int superReducedString() {
    char* s = readline123();
    char* sao = reduce(s);
    puts(sao);

    free(s);
    s = NULL;
    return 0;
}