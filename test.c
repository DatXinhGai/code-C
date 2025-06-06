//
// Created by HP LAPTOP on 27/12/2024.
//
#include <stdio.h>
#include <string.h>
#include <assert.h>
// thử thêm dấu cách và không thêm ở chỗ scanf của c2 xem
// bản chất nhập c1 thì trong buffer còn lại \n
// không có dấu cách thì c2 nhận luôn \n
// vì đây là %c nên lấy cả \n ấy

void test() {
    char c1, c2;
    printf("1\n");
    scanf("%c", &c1);
    printf("%c\n", c1);
    printf("2\n");
    scanf(" %c", &c2);  // dấu cách dùng để bỏ mọi loại kí tự trống, kể cả \n, tab
    printf("%c", c2);
}



