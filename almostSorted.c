//
// Created by HP LAPTOP on 06/02/2025.
//
// ý tưởng là tìm cực trị
// không có cực trị thì đơn điệu, kiểm tra thằng đầu nhỏ hơn thằng cuối là xong
// có 2 hoặc 4 cực trị thì xét swap
// không thỏa mãn thì xét reverse, chỉ được có 1 end thôi,
// nếu thỏa mãn thì so 2 lân cận của start và end
// cứ bảo học toán làm gì bài này dùng đồ thị hàm số chả mạnh vch
#include <stdio.h>
#include <stdlib.h>

void xetalmostSorted(int* arr, int n) {
    int cuc_dai = 0;
    int cuc_tieu = 0;
    // đếm số cực đại và cực tiểu
    for (int i = 1; i < n - 1; i++) {
        if (arr[i] > arr[i - 1] && arr[i] > arr[i + 1]) {
            cuc_dai++;
        } else if (arr[i] < arr[i - 1] && arr[i] < arr[i + 1]) {
            cuc_tieu++;
        }
    }

    // đơn điệu thì kiểm tra đầu cuối là được
    if (cuc_dai == 0 && cuc_tieu == 0) {
        if (arr[1] < arr[n - 2]) {
            puts("yes");
            return ;
        }
    }

    // co 2 hoac 4 ki di
    // cuc tri dau tien tu trai sang phai la cuc tieu
    if ((cuc_dai == 1 && cuc_tieu == 1) || (cuc_dai == 2 && cuc_tieu == 2) ) {
        int cs1 = -1;
        int cs2 = -1;
        // tìm cực đại ngoài cùng bên trái và cực tiểu ngoài cùng bên phải
        for (int i = 1; i < n - 1; i++) {
            if (arr[i] > arr[i - 1] && arr[i] > arr[i + 1]) {
                cs1 = i;
                break;
            }
        }
        for (int i = n - 2; i >= 1; i--) {
            if (arr[i] < arr[i - 1] && arr[i] < arr[i + 1]) {
                cs2 = i;
                break;
            }
        }
        // có 2 cực trị mà cực đại trừ cực tiểu  0 thì cook
        if (cuc_dai != 1 || (cs1 - cs2 != -1 && cs1 - cs2 != -2)) {
            // hình dung khi đổi chỗ 2 số
            // nếu có 2 cực trị thì cs1 chính là cs2 - 1 hoặc cs2 - 2;
            if (cs1 < cs2 && arr[cs1] < arr[cs2 + 1] && arr[cs1] >= arr[cs2 - 1] && arr[cs2] > arr[cs1 - 1]) {
                puts("yes");
                printf("swap %d %d", cs1, cs2);
                return ;
            }
        }


    }

    // reverse
    int start = -1;
    int end = -1;
    for (int i = 1; i <= n - 1; i++) {
        if (arr[i] < arr[i - 1]) {
            // tồn tại 2 dãy giảm
            if (end != -1) {    // 2 dãy khác nhau
                puts("no");
                return ;
            }
            if (start == -1) {  // bắt đầu dãy
                start = i - 1;
            }
        } else {
            // vị trí end ở cuối
            if (start != -1 && end == -1) {
                end = i - 1;
            }
        }
    }

    // reverse thì đồ thị phải đi lên
    if (arr[start] > arr[end + 1] || arr[end] < arr[start - 1]) {
        puts("no");
        return ;
    }
    puts("yes");
    printf("reverse %d %d", start, end);

}

int almostSorted() {
    int n;
    scanf("%d", &n);
    n += 2;
    int* arr = (int*)malloc(n*sizeof(int));
    arr[0] = -1;
    int max = -1;
    for (int i = 1; i < n - 1; i++) {
        scanf("%d", &arr[i]);
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    arr[n - 1] = max + 1;

    xetalmostSorted(arr, n);
    free(arr);
    arr = NULL;




    return 0;
}