//
// Created by HP LAPTOP on 03/02/2025.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/*
INPUT
khong 9 tam_hai:ba(4 nam).678

OUTPUT
0982345678

INPUT
09-tam_hai:ba(4 nam).6 bon_8

OUTPUT
0982345648
*/

char* readlinesdt() {
    int data_len = 0;
    int alloc_len = 1024;
    char* data = (char*)malloc(alloc_len);

    while (true) {
        char* cursor = data + data_len;
        char* line = fgets(cursor, alloc_len - data_len, stdin);
        if (!line) {
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
        temp = NULL;
    }
    if (data[data_len - 1] == '\n') {
        data_len--;
    }
    data[data_len] = '\0';
    data = (char*)realloc(data, data_len + 1);

    return data;
}



int giaiMaSdt() {
    const char* chu[10] = {"khong", "mot", "hai", "ba", "bon", "nam", "sau", "bay", "tam", "chin"};
    char* maHoa = readlinesdt();
    puts(maHoa);
    if (!maHoa) {
        fprintf(stdout, "Read unsuccessfully");
        exit(EXIT_FAILURE);
    }
    char* source_ptr = maHoa;
    char* dest_ptr = maHoa;

    if (*maHoa != '\0') {
        while (*source_ptr != '\0') {
            if (*source_ptr >= '0' && *source_ptr <= '9') {
                *dest_ptr = *source_ptr;
                dest_ptr++;
                source_ptr++;
            } else if (*source_ptr >= 'a' && *source_ptr <= 'z') {
                for (int i = 0; i < 10; i++) {
                    if (strstr(source_ptr, chu[i]) == source_ptr && (*(source_ptr + strlen(chu[i])) < 'a' || *(source_ptr + strlen(chu[i])) > 'z')) {
                        *dest_ptr = i + '0';
                        dest_ptr++;
                        source_ptr += strlen(chu[i]);
                        break;
                    }
                }
            } else {
                source_ptr++;
            }
        }
        *dest_ptr = '\0';
    }
    puts(maHoa);

    free(maHoa);
    maHoa = NULL;

    return 0;
}