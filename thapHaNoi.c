//
// Created by HP LAPTOP on 26/02/2025.
//
#include <stdio.h>
#include <stdlib.h>
/*
 * các bước gồm"
 * move n - 1 cái từ A sang C lấy B làm trung gian
 * move 1 cái từ A sang B lấy C làm trung gian
 * move n - 1 cái từ C sang B lấy A làm trung gian
 */

void moveTower(int n, char A, char B, char C) {
    if (n == 1) {
        printf("Move from %c to %c\n", A, B);
        return ;
    }

    moveTower(n - 1, A, C, B);
    moveTower(1, A, B, C);
    moveTower(n - 1, C, B, A);
}

int thapHaNoi() {
    int n;
    scanf("%d", &n);

    moveTower(n, 'A', 'B', 'C');
    return 0;

}