//
// Created by HP LAPTOP on 13/01/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// độ phức tạp O(1)
// có thể tìm ct tổng quát cho k cột
long double capSoNhan(int n, long double xs) {
    long double kq = pow(1.0/3,n) * (xs - 1.0/4);
    return (n%2 == 0)? kq  + 1.0/4 : -kq + 1.0/4;
}

// còn cách nữa là dùng ma trận 2x2 trong đại số, chéo hóa là xong thôi
// a11 = 0, a12 = 1/3, a21 = 1, a22 = 2/3


// độ phức tạp O(3^n)
// không dùng được khi xs ban đầu khác 1
// khí số cột nhiều thì việc truyền vào và tính chết nhọc
long double deQuyEch(int hang, int cot) {
    if (hang == 0) {
        if (cot == 0) {
            return 1.0;
        }
        return 0.0;
    }
    return 1.0/3*(deQuyEch(hang - 1, (cot + 1)%4) + deQuyEch(hang - 1, (cot + 2)%4) + deQuyEch(hang - 1, (cot + 3)%4));
}



// độ phức tạp O(1)
// có thể mở rộng ra nhiều cột
long double quyHoachDongEch(int hang)  {
    if (hang == 0) {
        return 1;
    } else {
        long double **bang = malloc(hang*sizeof(long double*));
        for (int i = 0; i < hang; i++) {
            *(bang + i) = malloc(4*sizeof(long double));
        }
        bang[0][0] = 1;
        for (int i = 1; i < 4; i++) {
            bang[0][i] = 0;
        }
        for (int i = 1; i < hang; i++) {
            for (int j = 0; j < 4; j++) {
                bang[i][j] = 1.0/3*(bang[i - 1][(j + 1)%4] + bang[i - 1][(j + 2)%4] + bang[i - 1][(j + 3)%4]);
            }
        }
        return bang[hang - 1][0];
    }
}

void chuEchCon() {
    int n;
    scanf("%d", &n);
    long double xs;
    //long double kq = deQuyEch(n, 0);
    //long double kq = capSoNhan(n - 1, xs);
    long double kq = quyHoachDongEch(n + 1);
    printf("%.10Lg", kq);
}