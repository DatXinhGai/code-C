//
// Created by HP LAPTOP on 02/03/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


char* congXau(char* s1, char* s2, int max_len) {
    int s1_len = strlen(s1);
    int s2_len = strlen(s2);
    if (s1_len < max_len) {
        s1 = (char*)realloc(s1, max_len + 1);
        memmove(s1 + max_len - s1_len, s1, s1_len + 1);
        memset(s1, '0', max_len - s1_len);
    } else if (s2_len < max_len) {
        s2 = (char*)realloc(s2, max_len + 1);
        memmove(s2 + max_len - s2_len, s2, s2_len + 1);
        memset(s2, '0', max_len - s2_len);
    }
    char* sum = (char*)malloc(max_len + 1);
    sum[max_len] = '\0';
    int cs = max_len - 1;
    int du = 0;
    while (cs >= 0) {
        sum[cs] = (s1[cs] + s2[cs] - 2*'0')%10 + '0' + du;
        du = (s1[cs] + s2[cs] - 2*'0')/10;
        cs--;
    }
    if (du == 1) {
        sum = (char*)realloc(sum, max_len + 2);
        memmove(sum + 1, sum, max_len + 1);
        *sum = '1';
    }
    free(s1);
    free(s2);
    return sum;
}

char* convertToNums(char* s, int* value) {
    char* re = strdup(s);
    char* ptr = re;
    while (*ptr) {
        *ptr += value[*ptr - 'A'];
        ptr++;
    }
    return re;
}

bool dienSo_re(char* s1, char* s2, char* tong, int* value, int max_len, int dem, char character) {
    if (dem == 26) {
        char* nums1 = convertToNums(s1, value);
        char* nums2 = convertToNums(s2, value);
        char* numsSum = convertToNums(tong, value);
        char* re = congXau(s1, s2, max_len);
        free(nums1);
        free(nums2);
        if (strcmp(numsSum, re) == 0) {
            free(re);
            return true;
        }
        free(re);
        return false;
    }
    for (int i = value[character - 'A']; i <= 9; i++) {
        value[character - 'A'] = i;
        if (dienSo_re(s1, s2, tong, value, max_len, dem + 1, character + 1) == true) {
            return true;
        }
    }
    return false;
}


int dienSo() {
    char* s1 = (char*)malloc(1024);
    char* s2 = (char*)malloc(1024);
    char* tong = (char*)malloc(1024);
    fgets(s1, 1023, stdin);
    fgets(s2, 1023, stdin);
    fgets(tong, 1023, stdin);
    s1[strcspn(s1, "\r\n")] = '\0';
    s2[strcspn(s2, "\r\n")] = '\0';
    tong[strcspn(tong, "\r\n")] = '\0';

    int* value = (int*)calloc(26, sizeof(int));
    value[*s1 - 'A'] = 1;
    value[*s2 - 'A'] = 1;
    value[*tong - 'A'] = 1;

}