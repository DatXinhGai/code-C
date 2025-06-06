//
// Created by HP LAPTOP on 03/01/2025.
//
#include <stdio.h>
#include <string.h>

struct StructExample {
    int i;
    float f;
    char str[20];
};


// union thường được dùng để xử lý các dữ liệu dạng variant hoặc các kiểu dữ liệu khác nhau nhưng chia sẻ cùng một vùng bộ nhớ
union UnionExample {
    int i;
    float f;
    char str[20];
};



void StructAndUnion() {
    printf("Size of Struct: %lu bytes\n", sizeof(struct StructExample));
    printf("Size of Union: %lu bytes\n", sizeof(union UnionExample));

    union UnionExample data;
    data.i = 10;    // lưu giá trị integer
    printf("data.i: %d\n", data.i);

    data.f = 220.5; // lưu giá trị float trước khi bị ghi đè
    printf("data.f: %f\n", data.f);

    strcpy(data.str, "Dat Xinh Gai");
    printf("data.str: %s\n", data.str);

}