//
// Created by HP LAPTOP on 27/02/2025.
//
#include <stdio.h>
#include <stdlib.h>

// việc ghi các ô quân hậu đi là cực hay vì biết trước số lần đi của hậu
// tuy nhiên việc làm như check hậu gây tốn thời gian vì phải check đi check lại nhiều lần
// có thể dùng mảng phụ ghi nhận các ô đã bị chiếm chỗ(chéo xuống hoặc chéo lên 1 ô)
// việc này giúp cho việc tra cứu giảm từ o(n) xuống còn o(1)
// sau khi đệ quy xong trả lại vị trí đó

void ghiNhan(int* arr, int n, int* count) {
 (*count)++;
 for (int i = 0; i < n; i++) {
     printf("%d ", arr[i] + 1);
 }
 puts("");
}

int checkHau(int* arr, int ucv, int dem) {
    // trả về 1 nếu j thuộc Sk
    // check xem có trường hợp nào mà không tấn công
    for (int i = 0; i < dem; i++) {
        if (ucv == arr[i] || abs(ucv - arr[i]) == dem - i - 1) {
            return 0;
        }
    }
    return 1;
}

// lấy đầu vào là cột i
void Hau(int* arr, int n, int i, int* count) {
    for (int j = 0; j < n; j++) {
        if (checkHau(arr, j, i + 1)) {
            arr[i] = j;
            if (i == n - 1) {
                ghiNhan(arr, n, count);
            } else {
                Hau(arr, n, i + 1, count);
            }
        }
    }
}


void HauToiUu(int* arr, int* clen, int* cxuong, int* ngang, int n, int i, int* count) {
    if (i == n) {
        ghiNhan(arr, n, count);
        return ;
    }

    for (int j = 0; j < n; j++) {
        if (!clen[i + j] && !cxuong[i - j + n - 1] && !ngang[j]) {
            clen[i + j] = 1;
            cxuong[i - j + n - 1] = 1;
            ngang[j] = 1;
            arr[i] = j;
            HauToiUu(arr, clen, cxuong, ngang, n, i + 1, count);
            clen[i + j] = 0;
            cxuong[i - j + n - 1] = 0;
            ngang[j] = 0;
        }
    }
}





int xepHau() {
    int n;
    scanf("%d", &n);

    // arr[i] nghĩa là chọn hàng arr[i] ở cột i + 1
    int* arr = (int*)malloc(n*sizeof(int));
    int count = 0;
    //Hau(arr, n, 0, &count);


    int* clen = (int*)calloc((2*n - 1), sizeof(int));
    int* cxuong = (int*)calloc((2*n - 1), sizeof(int));
    int* ngang = (int*)calloc(n, sizeof(int));
    // tối ưu nhưng không tối ưu
    HauToiUu(arr, clen, cxuong, ngang, n, 0, &count);
    if (count == 0) {
        puts("Không có lời giải!");
    }
     return 0;
}