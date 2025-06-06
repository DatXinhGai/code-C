//
// Created by HP LAPTOP on 04/02/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

void swap(int* a, int* b) {
    *a += *b;
    *b = *a - *b;
    *a -= *b;
}

int permute() {
    int n;
    scanf("%d", &n);

    int* arr = (int*)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
        printf("%d ", arr[i]);
    }
    puts("\n");


    while (true) {
        int mark = -1;
        for (int i = 0; i < n - 1; i++) {
            if (arr[i] < arr[i + 1]) {
                mark = i;
            }
        }
        if (mark == -1) {
            break;
        }
        // bản chất là dãy sau mark giảm, chạy từ cuối về mark + 1 để tìm
        // thằng bé nhất lớn hơn mark
        int chiSo;

        for (int i = n - 1; i >= mark + 1; i--) {
            if (arr[i] > arr[mark]) {
                chiSo = i;
                break;
            }
        }

        // swap không cần biến phụ
        swap(&arr[chiSo], &arr[mark]);

        // đổi chỗ 2 số thì phần sau theo thứ tự giảm dần
        // nên đảo ngược dãy là xong
        // không nên bubble sort vì phức tạp hơn
        /*
        int giua = (int)(ceil((n + mark)/2.0) - 1);
        for (int i = mark + 1; i <= giua; i++) {
            temp = arr[i];
            arr[i] = arr[n + mark - giua];
            arr[n + mark - giua] = temp;
        }
        */
        // cách sort trên khá hay nhưng dùng left right dễ hiểu hơn

        int left = mark + 1;
        int right = n - 1;
        while (left < right) {
            swap(&arr[left], &arr[right]);
            left++;
            right--;
        }

        for (int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        puts("\n");
    }


    free(arr);
    arr = NULL;
    return 0;
}