//
// Created by HP LAPTOP on 01/02/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000
#define MAX 400
#define COL 50

void createTest() {
    // tạo seed cho hàm rand
    srand(time(NULL));
    // đảm bảo cho các lần random là khác nhau

    for (int i = 0; i < SIZE; i++) {
        if (rand() % 10 == 0) {
            printf("-1 ");
        } else {
            printf("%d ", rand() % MAX + 1);
        }
        if (i % COL == 0) {
            printf("\n");
        }
    }
}
