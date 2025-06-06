//
// Created by HP LAPTOP on 07/01/2025.
//
#include <stdio.h>
#include <stdlib.h>


long long deToOc(long long n) {
    return n/8*10 + n%8;
}

char* deToBin(long long n) {
    int max_length = 1024;
    int cur_length = 0;
    char* mang = (char*)malloc(max_length);
    while (n > 0) {
        mang[cur_length++] = (n%2) + '0';
        n /= 2;
        if (cur_length == max_length) {
            max_length <<= 1;
            mang = (char*)realloc(mang, max_length);
        }
    }
    char temp;
    for (int i = 0; i <= (cur_length - 1)/2; i++) {
        temp = mang[i];
        mang[i] = mang[cur_length - 1 - i];
        mang[cur_length - 1 - i] = temp;
    }
    mang[cur_length] = '\0';

    return mang;
}

void doiCoSo() {
    long long n;
    scanf("%lld", &n);
    char* kq = deToBin(n);
    puts(kq);
}
