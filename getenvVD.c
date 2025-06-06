//
// Created by HP LAPTOP on 04/01/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


// perror in ra thông điệp ứng với mã lỗi trong biến toàn cục errno
// tham số là thông điệp bổ sung

// strerror trả về thông điệp lỗi dạng chuổi tương ứng với mã lỗi

void getenvVD() {
    FILE* file = fopen(getenv("OUTPUT_PATH"), "w");
    if (!file) {
        perror("Error opening file\n");
        printf("Error: %s\n", strerror(ENOENT));
    }
    fprintf(file, "Dat Dep Trai");
    fclose(file);
}