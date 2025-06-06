//
// Created by HP LAPTOP on 05/01/2025.
//
#include <stdio.h>
#include <errno.h>


/* cú pháp: int ungetc(int c, FILE *stream)
 * nếu luồng đã đóng hoặc đẩy kí tự không hợp lệ, trả về EOF;
 * nếu thành công trả về kí tự đẩy vào
 */


void ungetC() {
    FILE *fptr = fopen("output.txt", "r+");
    if (!fptr) {
        perror("Error opening file");
    }

    int c = fgetc(fptr);

    if (c != EOF) {
        printf("kí tự đọc được: %c\n", c);

        ungetc(c, fptr);

        int c2 = fgetc(fptr);
        printf("kí tự đọc lại: %c\n", c2);
    }
    fclose(fptr);
}