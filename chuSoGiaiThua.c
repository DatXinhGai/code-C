//
// Created by HP LAPTOP on 26/12/2024.
//
#include <stdio.h>
#include <math.h>

#define PI 3.14159
#define e 2.71828


// ĐỆ QUY
double deQuyGiaiThua(int n, double tong) {
    if (n <= 1) {
        return tong;
    } else {
        return deQuyGiaiThua(n - 1, tong + log10(n));
    }
}





// TỔNG TÍCH PHÂN
// chỉ nên dùng khi n lớn
// dùng tích phân xác định bằng cách biến đổi tổng tích phân,
// biểu thức tích phân là suy rộng, tính được nguyên hàm
// dùng vô cùng bé và L'Hopital kết luận xln(x) -> 0 khi x -> 0.
// biểu thức cuối cùng là 10^(n*(log10(n) - 1)
double tongTichPhanGiaiThua(int n) {
    double kq = n * 1.0 * (log10(n) - 1/(log(10)));
    return kq;
}


// XẤP XỈ STIRLING
double xapXiStirlingGiaiThua(int n) {
    return log10(sqrt(2*PI*n)) + n * log10(n/e);
}



void chuSoGiaiThua() {
    int n;
    scanf("%d", &n);
    double kq1 = tongTichPhanGiaiThua(n);
    double kq2 = xapXiStirlingGiaiThua(n);
    double kq3 = deQuyGiaiThua(n, 0);
    //double kq4 = deQuy(n, tong);
    printf("Tổng tích phân: %lf\n", kq1);
    printf("Xấp xỉ stirling: %lf\n", kq2);
    printf("%lf", kq3);
}