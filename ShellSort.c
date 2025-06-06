//
// Created by HP LAPTOP on 20/02/2025.
//
#include <stdio.h>
#include <stdlib.h>

int ShellSort() {
    int n;
    scanf("%d", &n);
    int* arr = (int*)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    int gap, i, j;
    for (gap = n/2; gap > 0; gap /= 2) {
        for (i = gap; i < n; i++) {
            for (j = i - gap; j >= 0 && arr[j] > arr[j + gap]; j -= gap) {
                arr[j] ^= arr[i];
                arr[i] ^= arr[j];
                arr[j] ^= arr[i];
            }
        }
    }

    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    return 0;
}