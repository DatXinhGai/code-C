//
// Created by HP LAPTOP on 12/02/2025.
//
#include <stdio.h>
#include <stdlib.h>


// cách khác là dùng priority queue

int tongChuSo(int n) {
    if (n == 0) {
        return 0;
    }
    return n%10 + tongChuSo(n/10);
}

int digitMapping() {
    int n;
    scanf("%d", &n);
    int* nums = (int*)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) {
        scanf("%d", &nums[i]);
    }

    int tongcs = -1;
    for (int i = 0;  i < n; i++) {
        if (tongChuSo(nums[i]) > tongcs) {
            tongcs = tongChuSo(nums[i]);
        }
    }

    int* map = (int*)calloc(tongcs, sizeof(int));

    int max = -1;
    for (int i = 0; i < n; i++) {
        int cs = tongChuSo(nums[i]) - 1;
        if (map[cs] > 0) {
            if (nums[i] + map[cs] > max) {
                max = nums[i] + map[cs];
            }
        } else {
            map[cs] = nums[i];
        }
        if (nums[i] > map[cs]) {
            map[cs] = nums[i];
        }
    }

    printf("%d", max);

    free(nums);
    nums = NULL;

    free(map);
    map = NULL;

    return 0;
}