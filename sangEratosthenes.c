//
// Created by HP LAPTOP on 07/03/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */


int* closestPrimes(int left, int right, int* returnSize) {
    *returnSize = 2;

    int* re = (int*)malloc(2*sizeof(int));
    for (int i = 0; i < 2; i++) {
        re[i] = -1;
    }

    if (right - left < 2) {
        return re;
    }

    int* preCompute = (int*)calloc(right + 1, sizeof(int));

    int top = (int)ceil(sqrt(right));
    for (int i = 2; i <= top; i++) {
        if (!preCompute[i]) {
            for (int j = i*i; j <= right; j += i) {
                preCompute[j] = 1;
            }
        }
    }

    int cs1 = (left > 1)? left : 2;
    int cs2 = left;
    int min = right;

    while (true) {
        while (cs1 < right && preCompute[cs1]) {
            cs1++;
        }
        if (cs1 == right) {
            return re;
        }
        cs2 = cs1 + 1;
        while (cs2 <= right && preCompute[cs2]) {
            cs2++;
        }
        if (cs2 == right + 1) {
            return re;
        }
        int sub = cs2 - cs1;
        if (sub < min) {
            min = sub;
            re[0] = cs1;
            re[1] = cs2;
        }

        cs1 = cs2;
    }

}