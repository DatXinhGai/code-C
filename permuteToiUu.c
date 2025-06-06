//
// Created by HP LAPTOP on 05/02/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swapBitwise(int* a, int* b) {
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}
void printPermute(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i] + 1);
    }
    puts("");
}


int checkPermute(int *arr, int dem, int *temp, int ucv) {
    if (temp[ucv] == 0) {
        return 1;
    }
    return 0;
}

void permuteRecurse(int *arr, int n, int *temp, int i) {

    if (i == n) {
        printPermute(arr, n);
        return ;
    }
    for (int ucv = 0; ucv < n; ucv++) {
        if (checkPermute(arr, i + 1, temp, ucv)) {
            temp[ucv] = 1;
            arr[i] = ucv;

            permuteRecurse(arr, n, temp, i + 1);

            temp[ucv] = 0;
        }
    }
}

int permuteToiUu() {
    int n;
    scanf("%d", &n);

    int* arr = (int*)malloc(n*sizeof(int));
    int* temp =(int*)calloc(n, sizeof(int));
    int* sao = (int*)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
        sao[i] = arr[i];
    }
    clock_t start, end;

    permuteRecurse(sao, n, temp, 0);

    /*
    start = clock();
    do {
        for (int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        puts("");

        int mark = -1;
        // chạy ngược thì tìm lần tăng đầu là break (độ phức tạp dao động từ 1 đến n)
        // chạy xuôi thì cố định là n
        for (int i = n - 2; i >= 0; i--) {
            if (arr[i] < arr[i + 1]) {
                mark = i;
                break;
            }
        }
        if (mark == -1) {
            break;
        }

        // tìm ngược về đầu tìm thằng đầu tiên lớn hơn arr[mark] thì sẽ là thằng nhỏ nhất lớn hơn arr[mark]
        for (int i = n - 1; i >= 1; i--) {
            if (arr[i] > arr[mark]) {
                swapBitwise(&arr[mark], &arr[i]);
                break;
            }
        }

        int left = mark + 1;
        int right = n - 1;

        // swap từ mark + 1 đến cuối thì sẽ được thứ tự tăng dần
        while (left < right) {
            swapBitwise(&arr[left], &arr[right]);
            left++;
            right--;
        }
    } while (1);
    end = clock();
    printf("%lf", (double)(end - start)/CLOCKS_PER_SEC);
    */
    free(arr);
    arr = NULL;
    return 0;
}