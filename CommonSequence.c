#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// nếu giống thì lấy
// nếu không giống thì lấy 1 trong 2
// có 2 lựa chọn thì chọn cái ngắn hơn
char* shortestCommonSupersequence(char* s1, char* s2) {
    if (!*s2) {
        return s1;
    }
    if (!*s1) {
        return s2;
    }
    int s1_len = strlen(s1);
    int s2_len = strlen(s2);
    int row = s1_len + 1;
    int col = s2_len + 1;

    int** dp = (int**)malloc(row*sizeof(int*));
    for (int i = 0; i < row; i++) {
        dp[i] = (int*)malloc(col*sizeof(int));
    }

    for (int i = 0; i < row; i++) {
        dp[i][0] = i;
    }

    for (int i = 0; i < col; i++) {
        dp[0][i] = i;
    }

    for (int i = 1; i < row; i++) {
        for (int j = 1; j < col; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = 1 + ((dp[i - 1][j] <= dp[i][j - 1])? dp[i - 1][j] :  dp[i][j - 1]);
            }
        }
    }

    char* re = (char*)malloc(s1_len + s2_len + 1);
    int cs_row = s1_len;
    int cs_col = s2_len;
    char* ptr = re;

    while (cs_row > 0 && cs_col > 0) {
        if (s1[cs_row - 1] == s2[cs_col - 1]) {
            *re = s1[cs_row - 1];
            cs_row--;
            cs_col--;
        } else if (dp[cs_row][cs_col - 1] < dp[cs_row - 1][cs_col]) {
            *re = s2[cs_col - 1];
            cs_col--;
        } else {
            *re = s1[cs_row - 1];
            cs_row--;
        }
        re++;
    }

    while (cs_row > 0) {
        *re = s1[cs_row - 1];
        re++;
        cs_row--;
    }

    while (cs_col > 0) {
        *re = s2[cs_col - 1];
        re++;
        cs_col--;
    }

    *re = '\0';

    ptr = (char*)realloc(ptr, strlen(ptr) + 1);
    strrev(ptr);
    return ptr;

}

char* shortestCommonSupersequence2(char* s1, char* s2) {

}

int CommonSequence() {
    char* s1 = (char*)malloc(1024);
    fgets(s1, 1023, stdin);
    s1[strcspn(s1, "\r\n")] = '\0';

    char* s2 = (char*)malloc(1024);
    fgets(s2, 1023, stdin);
    s2[strcspn(s2, "\r\n")] = '\0';

    char* re = shortestCommonSupersequence(s1, s2);
    puts(re);
}