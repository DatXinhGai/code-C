#include <stdio.h>
#include <stdlib.h>

// VÒNG LẶP
// nhược điểm của cách này là với n và k lớn thì có thể bị tràn
int vongLapnCk(int k, int n) {
    if (k == 0) {
        return 1;
    } else if (k == 1) {
        return n;
    } else {
        int kGiaiThua = 1;
        int nGiaiThua = 1;

        for (int i = 2; i <= k; i++) {
            kGiaiThua *= i;
        }

        for (int i = n - k + 1; i <= n; i++) {
            nGiaiThua *= i;
        }
        int kq = nGiaiThua / kGiaiThua;
        return kq;
    }
}



// ĐỆ QUY TOP DOWN
// nhược điểm là khi số lần gọi hàm nhiều khi n và k lớn (Top Down)
int congThucnCk(int k, int n) {
    if (k == 0 || k == n) {
        return 1;
    } else if (k == 1) {
        return n;
    }else {
        return congThucnCk(k, n - 1) + congThucnCk(k - 1, n - 1);
    }
}


// QUY HOẠCH ĐỘNG
// cột đầu trừ thằng đầu ra là không hợp lệ, cho bằng 0 hết (dùng calloc cũng được nhưng for thì tối ưu max)
int quyHoachDongnCk(int k, int n) {
    if (k == 0 || k == n) {
        return 1;
    } else if (k == 1) {
        return n;
    } else {
        if (k > n/2) {
            k = n - k;  // tối ưu tạo bảng ít hàng nhất
        }
        // có thể calloc
        int **bang = (int**)malloc(k*sizeof(int*));
        for (int i = 0; i < k; i++) {
            *(bang + i) = (int*)malloc(n*sizeof(int));
            bang[i][0] = 0;
        }
        for (int i = 0; i < n; i++) {
            bang[0][i] = i + 1;
        }
        bang[0][0] = 1;

        for (int i = 1; i < k; i++) {
            for (int j = 1; j < n; j++) {
                bang[i][j] = bang[i - 1][j - 1] + bang[i][j - 1];
            }
        }
        int kq = bang[k - 1][n - 1];

        // giải phóng bộ nhớ
        for (int i = 0; i < k; i++) {
            if (*(bang + i)) {
                free(*(bang + i));
                *(bang + i) = NULL;
            }
        }
        if (bang) {
            free(bang);
            bang = NULL;
        }
        return kq;
    }
}

void nCk() {
    int k, n;
    printf("Nhập k và n: ");
    scanf("%d %d", &k, &n);
    int kq1 = vongLapnCk(k, n);
    int kq2 = congThucnCk(k, n);
    int kq3 = quyHoachDongnCk(k, n);
    printf("Vòng lặp: %d\n", kq1);
    printf("Đệ quy: %d\n", kq2);
    printf("Quy hoạch động: %d\n", kq3);
}

