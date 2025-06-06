//
// Created by HP LAPTOP on 01/03/2025.
//
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
#include <stdio.h>
#include <stdlib.h>
 /*
int* applyOperations(int* nums, int numsSize, int* returnSize) {
    *returnSize = numsSize;
    int cs = 0;
    int* re = (int*)malloc(numsSize*sizeof(int));

    for (int i = 0; i < numsSize - 1; i++) {
        if (nums[i] == 0) {
            continue;
        } else if (nums[i] == nums[i + 1]) {
            nums[i] <<= 1;
            nums[i + 1] = 0;
            re[cs++] = nums[i];
            i++;
        } else {
            re[cs++] = nums[i];
        }
    }

    if (nums[numsSize - 1] != 0) {
        re[cs++] = nums[numsSize - 1];
    }

    memset(re + cs, 0, (numsSize - cs)*sizeof(int));

    return re;
}
*/

int* applyOperations(int* nums, int numsSize, int* returnSize) {
    *returnSize = numsSize;

    int j = 0;
    // nums[j] luôn là 0 vì nếu không phải thì ngay trước đó
    // 1 là i = j thì j++ rồi
    // 2 là nếu trước j có số 0 thì đáng ra nó phải được swap rồi
    for (int i = 0; i < numsSize - 1; i++) {
        if (nums[i] == nums[i + 1]) {
            nums[i] <<= 1;
            nums[i + 1] = 0;
        }
        if (nums[i] != 0) {
            int temp = nums[i];
            nums[i] = nums[j];
            nums[j] = temp;
            j++;
        }
    }
    if (nums[numsSize - 1] > 0) {
        int temp = nums[numsSize - 1];
        nums[numsSize - 1] = nums[j];
        nums[j] = temp;
    }

    return nums;
}