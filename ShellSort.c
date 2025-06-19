//
// Created by HP LAPTOP on 20/02/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int ShellSort() {

    clock_t start, end;

    int n;
    scanf("%d", &n);
    int* arr = (int*)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    int gap, i, j;
    int upper = (int)pow(floor(log2(n)), 2.0);

    start = clock();
    // lấy dạng 2^k - 1 cho cận tồi nhất là O(n^(3/2)) thôi
    // còn 2^k hoặc n/2 thì là O(n^2)
    int temp;
    for (gap = upper - 1; gap > 0; gap = (gap == 1)? 0 : ((gap + 1) >> 1)) {
    //for (gap = n/2; gap > 0; gap /= 2) {
        for (i = gap; i < n; i++) {
            for (j = i - gap; j >= 0 && arr[j] > arr[j + gap]; j -= gap) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
                // nhanh hơn cả bitwise

                /*
                arr[j] ^= arr[i];
                arr[i] ^= arr[j];
                arr[j] ^= arr[i];
                */
            }
        }
    }

    /*
    for (gap = upper - 1; gap > 0; gap = (gap == 1)? 0 : ((gap + 1) >> 1)) {
        for (i = gap; i < n; i++) {
            int temp = arr[i];
            j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
    }
    */
    end = clock();

    double tim = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %lf\n", tim);

    /*
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    */

    free(arr);
    return 0;
}