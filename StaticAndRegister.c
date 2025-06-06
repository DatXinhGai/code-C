//
// Created by HP LAPTOP on 03/01/2025.
//

#include <stdio.h>

// biến register không thể lấy được địa chỉ
// không được đảm bảo lưu trong thanh ghi, quyết định là của trình biên dịch
void example_register() {
    register int i; // yêu cầu lưu trong thanh ghi
    for (i = 0; i < 10; i++) {
        printf("%d ", i);
    }
}

/*
* biến static có scope cục bộ nhưng lifetime kéo dài suốt chương trình
* biến static cục bộ chỉ truy cập trong hàm được khai báo, nhưng giá trị
không bị reset sau hàm kết thúc
* biến static toàn cục chỉ có thể được sử dụng trong file nơi nó được khai báo
*/
void example_static() {
    static int count = 0;   // biến static, giá trị được lưu lại
    count++;
    printf("Count: %d\n", count);
}




void StaticAndRegister() {
    example_register();
    printf("\n");
    for (int i = 0; i < 10; i++) { example_static(); };
}

