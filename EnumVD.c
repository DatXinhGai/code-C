//
// Created by HP LAPTOP on 02/01/2025.
//
#include <stdio.h>

// định nghĩa các quyền với flag bit
// kết hợp với bitwise
enum AccessRight {
    READ = 1,
    WRITE = 2,
    EXECUTE = 4,
};

void AccessRight() {
    int userPermission = READ | WRITE;
    if (userPermission & READ) {
        printf("You can READ this document\n");
    }
    if (userPermission & WRITE) {
        printf("You can WRITE this document\n");
    }
    if (userPermission & EXECUTE) {
        printf("You can EXECUTE this document\n");
    }
}

