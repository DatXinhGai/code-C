//
// Created by HP LAPTOP on 28/12/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int LuyThua2(int x) {
    return x & (x - 1);
}

int demBit1(int x) {
    int count = 0;
    while (x > 0) {
        x = x & (x - 1);
        count++;
    }
    return count;
}

int LuyThua2Max(int x) {
    int n = 1;
    while (n <= x) {
        n <<= 1;
    }
    n >>= 1;
    return n;
}

int ChiaHetLuyThua2Max(int x, int k) {
    return x &~(k - 1);
}

int ChiaHet2(int x) {
    return x & 1;
}

int DaoBit1ThapNhat(int x) {
    return x & (x - 1);         // Loại bỏ cờ(flag) trong các thuật toán bitmask
}

int TimPhanTuXuatHienLe(int n, int* mang) {
    int kq = 0;
    for (int i = 0; i < n; i++) {
        kq ^= mang[i];
    }
    return kq;
}

int DaoBitKBatKi(int n, int k) {
    return n &~(1 << k);
}

bool kiemTraBitDoiXung(int n) {
    int dai = 0;
    if (n == 0) {
        return true;
    }
    while (n > 0) {
        n >>= 1;
        dai++;
    }
    int trai = dai - 1;
    int phai = 0;
    while (trai > phai) {
        if (( (n >> trai) & 1 ) ^ ( (n >> phai) & 1 )) {
            return false;
        }
        trai--;
        phai++;
    }
    return true;
}

void Bitwise() {
    int luaChon;
    int n;
    printf("Lựa chọn: \n");
    printf("1. Kiểm tra lũy thừa của 2\n");
    printf("2. Đếm số bit 1\n");
    printf("3. Tìm lũy thừa 2 lớn nhất <= n\n");
    printf("4. Tìm số chia hết cho 1 lũy thừa của 2 lớn nhất <= n\n");
    printf("5. Kiểm tra 1 số có chia hết cho 2 không\n");
    printf("6. Tìm số xuất hiện lẻ lần trong mảng gồm n số\n");
    printf("7. Kiểm tra 1 số có dãy bit có phải palindrome không.\n");
    scanf("%d", &luaChon);
    puts("Nhập n");
    scanf("%d", &n);

    switch (luaChon) {
        case 1:
            if (!LuyThua2(n)) {
                printf("%d là 1 lũy thừa của 2", n);
            } else {
                printf("%d không là lũy thừa của 2", n);
            }
            break;
        case 2:
            printf("Số bit 1 là: %d", demBit1(n));
            break;
        case 3:
            printf("Số cần tìm là: %d", LuyThua2Max(n));
            break;
        case 4:
            int k;
            printf("Nhập lũy thừa của 2: ");
            scanf("%d", &k);
            printf("Số cần tìm là: %d", ChiaHetLuyThua2Max(n, k));
            break;
        case 5:
            if (!ChiaHet2(n)) {
                printf("%d chia hết cho 2", n);
            } else {
                printf("%d không chia hết cho 2", n);
            }
            break;
        case 6:
            int *mang = (int*)malloc(n*sizeof(int));
            if (!mang) {
                fprintf(stderr, "Tràn ngập bộ nhớ nhớ nhớ nhớ e!");
                exit(EXIT_FAILURE);
            }
            for (int i = 0; i < n; i++) {
                scanf("%d", &mang[i]);
            }
            printf("Phần tử cần tìm là: %d", TimPhanTuXuatHienLe(n, mang));
            free(mang);
            mang = NULL;
            break;
        case 7:
            if (kiemTraBitDoiXung(n)) {
                printf("Số %d có chuỗi bit là palindrome!", n);
            } else {
                printf("Số %d có chuỗi bit không đối xứng", n);
            }
    }
}