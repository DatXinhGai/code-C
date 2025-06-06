//
// Created by HP LAPTOP on 04/01/2025.
//




// là chỉ thị tiền xử lí được sử dụng để kiểm soát việc biên dịch có điều kiện

/* ifdef (if defined)
 * kiểm tra một macro (biến hoặc định nghĩa tiền xử lí) đã được định
 * nghĩa hay chưa bằng cách sử dụng #define
 * nếu macro đã được định nghĩa, đoạn mã bên trong #ifdef sẽ được biên dịch
*/

/* ifndef (if not defined)
 * điều kiện ngược lại với ifdef
*/

// endif để kết thúc 2 cái trên

#include <stdio.h>

#define MACRO_NAME

void macro() {
    #ifdef MACRO_NAME
        // Code sẽ được biên dịch nếu MACRO_NAME đã được định nghĩa
        printf("Macro đã được định nghĩa");
    #endif
}
