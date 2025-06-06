//
// Created by HP LAPTOP on 26/02/2025.
//
#include <stdio.h>
#include <stdbool.h>

// phải khai báo trước khi dùng
bool chan(int);
bool le(int);

bool chan(int n) {
    if (n == 0) {
        return true;
    }
    return le(n - 1);
}

bool le(int n) {
    if (n == 0) {
        return false;
    }
    return chan(n - 1);
}

int chanLe() {
    int n;
    scanf("%d", &n);

    bool isEven = chan(n);
    if (isEven) {
        puts("n là số chẵn");
    } else {
        puts("n là số lẻ");
    }

    return 0;
}