//
// Created by HP LAPTOP on 24/02/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * chia ra làm 3 phần, phần mid không cần sort nữa
 * gọi đệ quy để sort left và right
 * tạo ra cây với left và right tiếp tục mở rộng
 * tốn thêm mảng phụ để lưu trữ, cũng như mất thời gian để cấp phát,copy và realloc
 * mất đi lợi thế so với mergesort là bộ nhớ
 */
int* QuickSort(const int* arr, int n) {
    int pivot = (n - 1)/2;
    int left_size = 0;
    int mid_size = 0;
    int right_size = 0;
    int* left = (int*)malloc(sizeof(int));
    int* mid = (int*)malloc(sizeof(int));
    int* right = (int*)malloc(sizeof(int));

    for (int i = 0; i < n; i++) {
        if (arr[i] < arr[pivot]) {
            left[left_size++] = arr[i];
            if ((left_size & left_size - 1) == 0) {
                left = (int*)realloc(left, left_size*2*sizeof(int));
            }
        } else if (arr[i] == arr[pivot]) {
            mid[mid_size++] = arr[i];
            if ((mid_size & mid_size - 1) == 0) {
                mid = (int*)realloc(mid, mid_size*2*sizeof(int));
            }
        } else {
            right[right_size++] = arr[i];
            if ((right_size & right_size - 1) == 0) {
                right = (int*)realloc(right, right_size*2*sizeof(int));
            }
        }
    }

    int* re = (int*)malloc(n*sizeof(int));
    int* temp1 = left;
    int* temp2 = right;
    int* ptr = re;
    if (left_size > 0) {
        left = QuickSort(left, left_size);
        memcpy(ptr, left, left_size*sizeof(int));
        ptr += left_size;
        free(left);
    }

    memcpy(ptr, mid, mid_size*sizeof(int));
    ptr += mid_size;
    free(mid);

    if (right_size > 0) {
        right = QuickSort(right, right_size);
        memcpy(ptr, right, right_size*sizeof(int));
        free(right);
    }
    free(temp1);
    free(temp2);

    return re;
}

/*
 * thao tác trực tiếp, không tốn thêm bộ nhớ phụ
 * chọn thằng cuối là pivot
 * duyệt, nếu thằng nào nhỏ hơn pivot thì đưa lên đầu và tăng biến đếm
 * đến cuối thì swap pivot về đúng chỗ của nó
 * chia 2 phần sort tiếp
 */
void QuickSortToiUu(int* arr, int low, int high) {
    if (low >= high) {
        return ;
    }

    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < arr[high]) {
            i++;
            if (i != j) {
                arr[i] ^= arr[j];
                arr[j] ^= arr[i];
                arr[i] ^= arr[j];
            }
        }
    }

    int pivotPos = i + 1;
    if (pivotPos != high) {
        arr[pivotPos] ^= arr[high];
        arr[high] ^= arr[pivotPos];
        arr[pivotPos] ^= arr[high];
    }

    QuickSortToiUu(arr, low, pivotPos - 1);
    QuickSortToiUu(arr, pivotPos + 1, high);
}

void DutchNationalFlag(int* arr, int low, int high) {
    if (low >= high) {
        return ;
    }

    int pivot = arr[high];
    // biến i lưu vị trị của left, k lưu vị trí của right, j là biến chạy
    int i = low, j = low, k = high;

    while (j <= k) {
        if (arr[j] < pivot) {
            if (i != j) {
                arr[i] ^= arr[j];
                arr[j] ^= arr[i];
                arr[i] ^= arr[j];
            }
            i++, j++;
        } else if (arr[j] > pivot) {    // không xảy ra trường hợp j = k, vì khi đó arr[j] == pivot;
            if (j != k) {
                arr[j] ^= arr[k];
                arr[k] ^= arr[j];
                arr[j] ^= arr[k];
            }
            k--;    // không được cộng j vì khi đó arr[k] chưa được xét
        } else {
            j++;
        }
    }

    DutchNationalFlag(arr, low, i - 1);
    DutchNationalFlag(arr, k + 1, high);
}



int xapXepNhanh() {
    int n;
    scanf("%d", &n);
    int* arr = (int*)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    /*
    int* re = QuickSort(arr, n);
    for (int i = 0; i < n; i++) {
        printf("%d ", re[i]);
    }
    free(re);
    re = NULL;
    */

    //QuickSortToiUu(arr, 0, n - 1);
    DutchNationalFlag(arr, 0, n - 1);

    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    free(arr);
    arr = NULL;


    return 0;
}