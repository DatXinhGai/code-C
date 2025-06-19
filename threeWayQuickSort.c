//
// Created by HP LAPTOP on 17/06/2025.
//
#include <stdio.h>
#include <stdlib.h>

#include <time.h>


void insertionSort(int* arr, int start, int end) {
    for (int i = start + 1; i <= end; i++) {
        int j = i - 1;
        while (j > start && arr[i] < arr[j]) {
            j--;
        }

        int temp = arr[i];

        for (int k = i - 1; k >= j; k--) {
            arr[k + 1] = arr[k];
        }

        arr[j] = temp;
    }
}

void dutchNationalFlag(int* arr, int left, int right, int* left_idx, int* right_idx, int pivot) {
    *left_idx = left;
    *right_idx = right;
    int idx = left;

    while (idx <= *right_idx) {
        if (arr[idx] < pivot) {
            int temp = arr[*left_idx];
            arr[*left_idx] = arr[idx];
            arr[idx] = temp;

            if (idx == *left_idx) {
                idx++;
            }

            (*left_idx)++;
        } else if (arr[idx] == pivot) {
            idx++;
        } else {
            int temp = arr[*right_idx];
            arr[*right_idx] = arr[idx];
            arr[idx] = temp;
            (*right_idx)--;
        }
    }
}


void threeWayQuickSort(int* arr, int left, int right) {
    if (left >= right) {
        return ;
    }

    int dif = right - left + 1;

    if (dif < 20) {
        return insertionSort(arr, left, right);
    }

    int rand_val = rand() % dif;

    int pivot = arr[left + rand_val];

    int left_index, right_index;

    dutchNationalFlag(arr, left, right, &left_index, &right_index, pivot);

    if (left_index - left < right - right_index) {
        threeWayQuickSort(arr, left, left_index - 1);
        threeWayQuickSort(arr, right_index + 1, right);
    } else {
        threeWayQuickSort(arr, right_index + 1, right);
        threeWayQuickSort(arr, left, left_index - 1);
    }

}


int threeWay() {

    srand(time(NULL));

    int n;
    scanf("%d", &n);

    int* arr = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        arr[i] = rand();
        //scanf("%d", &arr[i]);
    }

    clock_t start, end;

    start = clock();
    //insertionSort(arr, 0, n - 1);
    threeWayQuickSort(arr, 0, n - 1);
    end = clock();

    printf("Time: %lf\n", (end * 1.0 - start) / CLOCKS_PER_SEC);
    free(arr);

    return 0;
}