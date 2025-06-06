//
// Created by HP LAPTOP on 10/02/2025.
//
// ý tưởng là đánh dấu vị trí bắt đầu, kết thúc
// khi mà tổng âm thì cắt chuỗi vì không dại gì số đằng sau lại cộng với số âm
#include <stdio.h>
#include <stdlib.h>

// độ phức tạp o(n)
// xem xét nếu s[i - 1] + nums[i] thì s[i] = nums[i](tức là bắt đầu tổng mới)
// khi đó không dại gì cộng thêm s[i - 1] cả
// else s[i] = s[i - 1] + nums[i]

int tongMaxQHD(int* nums, int n) {
    int tong = nums[0];
    int cs_start = 0;
    int start_max = 0;
    int cs_end = 0;
    int max = nums[0];
    for (int i = 1; i < n; i++) {
        if (tong < 0) {
            tong = nums[i];
            cs_start = i;
        } else {
            tong += nums[i];
        }
        if (tong > max) {
            start_max = cs_start;
            max = tong;
            cs_end = i;
        }
    }
    return max;
}

int tongMaxPrefix(int* nums, int n) {
    if (n == 1) {
        return nums[0];
    }

    int sum = 0;
    int minPrefixSum = 0;
    int re = 0;

    // tìm dãy tiền tố có tổng min sau đó xét tổng từ đầu đến phần đó trừ đi prefix

    for (int i = 0; i < n; i++) {
        sum += nums[i];
        if (sum < minPrefixSum) {
            minPrefixSum = sum;
        } else if (sum - minPrefixSum > re) {
            re = sum - minPrefixSum;
        }
    }

    return re;
}

// phức tạp o(n^3) do vét hết các tổ hợp có thể
// chọn 2 thằng đầu và cuối sau đó lấy tổng của nó
int tongMaxBF(int* nums, int n) {
    int max = nums[0];
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            int tong = 0;
            for (int k = i; k <= j; k++) {
                tong += nums[k];
            }
            if (tong > max) {
                max = tong;
            }
        }
    }
    return max;
}

int tongMaxBFCT(int* nums, int n) {
    int max = nums[0];
    for (int i = 0; i < n; i++) {
        int tong = 0;
        for (int j = i; j < n; j++) {
            tong += nums[j];
            if (tong > max) {
                max = tong;
            }
        }
    }

    return max;
}

int tongTrai(int* nums, int trai, int phai) {
    int max = nums[phai];
    int tong = nums[phai];
    for (int i = phai - 1; i >= trai; i--) {
        tong += nums[i];
        if (tong > max) {
            max = tong;
        }
    }

    return max;
}

int tongPhai(int* nums, int trai, int phai) {
    int max = nums[trai];
    int tong = nums[trai];
    for (int i = trai + 1; i <= phai; i++) {
        tong += nums[i];
        if (tong > max) {
            max = tong;
        }
    }

    return max;
}

int tongGiua(int* nums, int trai, int phai) {
    if (trai == phai) {
        return nums[trai];
    }

    int giua = (trai + phai)/2;
    int s_left = tongGiua(nums, trai, giua);
    int s_right = tongGiua(nums, giua + 1, phai);
    int s_mid = tongTrai(nums, trai, giua) + tongPhai(nums, giua + 1, phai);
    int max;
    return ((max = (s_left > s_right)? s_left : s_right) > s_mid)? max : s_mid;
}

// chứng minh bằng quy nạp được rằng độ phức tạp là o(nlogn)
// dễ stackoverflow nhưng độ phức tạp thấp hơn 2 cách kia
// divide and conquer
// chia các trường hợp là kết thúc ở giữa, bắt đầu từ giữa + 1 hoặc bắt đầu, kết thúc từ 2 bên
// không tối ưu nhưng là 1 cách tốt để học tập
int tongMaxRecur(int* nums, int n) {
    return tongGiua(nums, 0, n - 1);
}

int chuoiConTongMax() {
    int n;
    scanf("%d", &n);
    int* nums = (int*)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) {
        scanf("%d", &nums[i]);
    }
    int re1 = tongMaxQHD(nums, n);
    int re2 = tongMaxBF(nums, n);
    int re3 = tongMaxBFCT(nums, n);
    int re4 = tongMaxRecur(nums, n);
    int re5 = tongMaxPrefix(nums, n);

    printf("%d %d %d %d %d", re1, re2, re3 ,re4, re5);


    free(nums);
    nums = NULL;
    return 0;
    //printf("%d %d", start_max, cs_end);
}