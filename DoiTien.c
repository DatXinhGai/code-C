//
// Created by HP LAPTOP on 02/01/2025.
//
#include <stdio.h>
#include <stdlib.h>


// dùng khi các mệnh giá không liên tục (1, 2, 5, 10, 20,...)
int DoiTienDeQuy(int n, int sum) {
    if (sum < 0) {
        return 0;
    } else if (n <= 0) {
        return 0;
    } else if (sum == 0) {
        return 1;
    } else {
        return DoiTienDeQuy(n - 1, sum) +DoiTienDeQuy(n, sum - n);
    }
}

// tốc độ cực nhanh và hiệu quả
long long DoiTienQuyHoachDong(int n, int sum) {
    long long **bang = (long long**)malloc((n + 1)*sizeof(long long*));
    if (!bang) {
        fprintf(stderr, "Allocated unsuccessfully!");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i <= n; i++) {
        bang[i] = (long long*)malloc((sum + 1)*sizeof(long long));
        if (!bang[i]) {
            fprintf(stderr, "Allocated unsuccessfully");
            exit(EXIT_FAILURE);
        }
    }
    for (int j = 1; j <= sum; j++) {
        bang[0][j] = 0;
    }
    bang[0][0] = 1;

    for (int i = 1; i <= n; i++) {
        bang[i][0] = 1;
        for (int j = 1; j <= sum; j++) {
            if (j < i) {    // tiền nhỏ hơn mệnh giá thì giảm mệnh giá xuống bằng tiền
                bang[i][j] = bang[j][j];
            } else {
                bang[i][j] = bang[i - 1][j] + bang[i][j - i];
                // không đổi bằng đồng đấy hoặc đổi bằng đồng đấy
            }
        }
    }
    long long kq = bang[n][sum];
    for (int i = 0; i < n; i++) {
        free(bang[i]);
        bang[i] = NULL;
    }
    free(bang);
    bang = NULL;
    return kq;


}

void DoiTien() {
    int n, sum;
    printf("Nhập loại tiền lớn nhất và số tiền:\n");
    scanf("%d%d",&n, &sum);
    //int kqDeQuy = DoiTienDeQuy(n, sum);
    long long kqQHD = DoiTienQuyHoachDong(n, sum);
    //printf("Đệ quy: %d\n", kqDeQuy);
    printf("Quy hoạch động: %lld\n", kqQHD);

}