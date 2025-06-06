//
// Created by HP LAPTOP on 23/02/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * SPIRIT là chia thật nhỏ dãy ra
 * sau đó merge 2 dãy dùng 2 con trỏ chạy
 * gồm 3 quy trình là sort left, right rồi merge
 * hình dung việc đệ quy sẽ tạo ra các cây với nhiều nhánh, trong đó nhánh left và right cứ tiếp tục mở rộng
 */


void Merge(int* arr, int start, int mid, int end) {
    int left_size = mid - start + 1;
    int right_size = end - mid;
    int left[left_size];
    int right[right_size];
    memcpy(left, arr + start, left_size*sizeof(int));
    memcpy(right, arr + mid + 1, right_size*sizeof(int));

    int cur_left = 0;
    int cur_right = 0;
    int index = 0;

    while (cur_left < left_size && cur_right < right_size) {
        if (left[cur_left] < right[cur_right]) {
            arr[start + index] = left[cur_left];
            index++;
            cur_left++;
        } else {
            arr[start + index] = right[cur_right];
            index++;
            cur_right++;
        }
    }

    if (cur_left == left_size) {
        for (int i = cur_right; i < right_size; i++) {
            arr[start + index] = right[i];
            index++;
        }
    } else {
        for (int i = cur_left; i < left_size; i++) {
            arr[start + index] = left[i];
            index++;
        }
    }
}

void MergeSort(int* arr, int start, int end) {
    if (start < end) {
        int mid = (end + start)/2;
        MergeSort(arr, start, mid);
        MergeSort(arr, mid + 1, end);
        Merge(arr, start, mid, end);
    }
}


int xapXepTron() {
    int n;
    scanf("%d", &n);
    int* arr = (int*)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    MergeSort(arr, 0, n - 1);


    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }


    free(arr);
    arr = NULL;
    return 0;
}
