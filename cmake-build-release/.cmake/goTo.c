//
// Created by HP LAPTOP on 05/01/2025.
//
#include <stdio.h>

void goTo() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%d %d\n", i, j);
            if (i == 3 && j == 3) {
                goto exit;
            }
        }
    }
    exit:

}