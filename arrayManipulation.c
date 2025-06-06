//
// Created by HP LAPTOP on 26/02/2025.
//
#include <stdio.h>
#include <stdlib.h>
/*
 * Starting with a 1-indexed array of zeros and a list of operations,
 * for each operation add a value to each array element between two given indices, inclusive.
 * Once all operations have been performed, return the maximum value in the array.
*/
// lưu ý bài không bắt chỉ ra chỗ max
// nếu làm bình thường chắc chắn TLE
// ở đây để ý rằng việc bắt đầu sẽ làm tăng thêm giá trị của cur
// và việc kết thúc sẽ làm giảm xuống
// tăng ở vị trí bắt đầu và giảm ở vị trí sau kết thúc
// duyệt tổng và tìm max
// o(n)
/*
int arrayManipulation() {
    int n, queries_rows;
    scanf("%d", &n);
    scanf("%d", &queries_rows);

    int** queries = (int**)malloc(q*sizeof(int))

}
*/
// sweep line