//
// Created by HP LAPTOP on 29/12/2024.
//

#include "LuyThuaNhanh.h"

#include <stdio.h>
#include <stdlib.h>

// Hàm tính lũy thừa nhanh (modular exponentiation)
long long power(long long a, long long b, long long m) {
    long long result = 1;
    a = a % m;
    while (b > 0) {
        if (b % 2 == 1) {
            result = (result * a) % m;
        }
        b = b >> 1;
        a = (a * a) % m;
    }
    return result;
}

void LuyThuaNhanh() {
    long long a;
    int n;  // số phần tử của mảng b
    scanf("%lld", &a);
    scanf("%d", &n);

    int* b = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        scanf("%1d", &b[i]);  // Đọc từng chữ số vào mảng b
    }

    // Tính b % 1140
    long long b_mod = 0;
    for (int i = 0; i < n; i++) {
        b_mod = (b_mod * 10 + b[i]) % 1140;
    }

    // Nếu b_mod bằng 0 và n lớn hơn 1, set b_mod bằng 1140
    if (b_mod == 0 && n > 1) {
        b_mod = 1140;
    }

    // Tính a^b_mod % 1337
    long long result = power(a, b_mod, 1337);

    // In kết quả
    printf("%lld\n", result);

    // Giải phóng bộ nhớ
    free(b);
}
