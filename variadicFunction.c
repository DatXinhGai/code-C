//
// Created by HP LAPTOP on 04/02/2025.
//
#include <stdio.h>
#include <stdarg.h>

void sumAndMax(int count, ...) {
    va_list args1, args2;   // khởi tạo biến args để xử lí các đối số
    va_start(args1, count);  // bắt đầu đọc các đối số sau count

    va_copy(args2, args1);   // copy đối số từ hàm 1 vào hàm 2 để dùng lại

    // tìm tổng
    int total = 0;

    for (int i = 0; i < count; i++) {
        total += va_arg(args1, int);     // lấy đối số kiểu int
    }
    printf("Tổng: %s", total);


    // tìm max
    int max;
    int temp;
    max = va_arg(args2, int);
    for (int i = 1; i < count; i++) {
        temp = va_arg(args2, int);
        if (temp > max) {
            max = temp;
        }
    }
    printf("Số lớn nhất là: %d", max);

    va_end(args1);   // kết thúc xử lí đối số
    va_end(args2);  // nhớ phải end hết không là bị leak bộ nhớ

}

int variadicFunction() {
    sumAndMax(4, 1, 2, 3, 4);
}