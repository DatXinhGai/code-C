//
// Created by HP LAPTOP on 28/12/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Một số tính chất
 *  a ^ a = 0
 *  a ^ (b ^ c) = a ^ c ^ b (kết hợp)
 *  a ^ b = b ^ a (giao hoán)
 *  a & a = a
 *  & có tính kết hợp và giao hoán như ^
 *  (a ^ b) + 2 * (a & b) = a + b

*/





int LuyThua2(int x) {
    return x & (x - 1);     // 2 số liên tiếp and nhau = 0 <=> số lớn là lũy thừa của 2
}

int demBit1(int x) {        // nếu tận cùng là 1 thì x - 1 tận cùng là 0
    int count = 0;          // tận cùng là & x - 1 trả về 0 hết
    while (x > 0) {         // hình dung 1000 và 0111 chạy được 1 lần
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
    return x &~(k - 1);     // bỏ tất cả bit bé
}                           // ví dụ 80 và 8 thì bỏ 3 bit cuối của 80
                            // phép sau là chuyển 3 bit cuối của 8 về 0
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

bool kiemTraBitDoiXung(int n) {     // a ^ a = 0 nên triệt tiêu phần tử xuất hiện chẵn
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

long long DaoBit(int n) {
    long long result;
    while (n > 0) {
        result = (result << 1) | (n & 1);   // giống đổi cơ số dùng đệ quy
        n >>= 1;
    }
    return result;
}

void tim2PhanTuXuatHienLe(int n, int *day) {
    int xor_result = 0;
    int num1, num2;
    for (int i = 0; i < n; i++) {
        xor_result ^= day[i];
    }
    int bit1_phai = xor_result ^ (xor_result & (xor_result -  1));
    // bỏ bit 1 đầu từ phải sang
    /*  hoặc int bit1_phai = xor_result & -xor_result;
     * bản chất -x = ~x + 1
     * nếu các bit từ phải sang bằng 0 đổi theo bù 2 cộng 1 cuối cùng vẫn là 0, 0 & 0 = 0
     * chỉ có bit 1 đầu là 1 & 1
     * các bit lớn hơn thì là a & ~a nên là 0;
     * biểu thức trên tìm bit 1 đầu tiên từ phải sang và trả về dãy bit có đúng bit này bật
     * tức là bit đầu tiền mà 2 số cần tìm khác nhau
     * chia day thành 2 phần theo tiêu chí này để xor với num1 và num2 thì num1 và num2 bị cách ly
     */
    for (int i = 0; i < n; i++) {
        if (day[i] & bit1_phai) {
            num1 ^= day[i];
        } else {
            num2 ^= day[i];
        }
    }
    printf("Hai số cần tìm là: %d %d", num1, num2);
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
    printf("8. Tìm 2 số xuất hiện lẻ lần trong 1 dãy n phần tử");


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
        case 8:
            int *day = (int*)malloc(n*sizeof(int));
            for (int i = 0; i < n; i++) {
                scanf("%d", &day[i]);
            }
            tim2PhanTuXuatHienLe(n, day);
            free(day);
        day = NULL;
    }
}