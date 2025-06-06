//
// Created by HP LAPTOP on 02/03/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_sub(char* s1, int off, int k) {
    if (k == 0) {
        puts("");
        return ;
    }
    off--;
    k--;
    print_sub(s1, off, k);
    printf("%c", *(s1 + off));
}

int subString() {
    char* s1 = (char*)malloc(1024);
    char* s2 = (char*)malloc(1024);
    fgets(s1, 1023, stdin);
    s1[strcspn(s1, "\r\n")] = '\0';
    fgets(s2, 1023, stdin);
    s2[strcspn(s2, "\r\n")] = '\0';

    int s1_len = strlen(s1);
    int s2_len = strlen(s2);

    int** dp = (int**)malloc(s1_len*sizeof(int*));
    int** row = (int**)malloc(s1_len*sizeof(int*));
    int** col = (int**)malloc(s2_len*sizeof(int*));
    for (int i = 0; i < s1_len; i++) {
        row[i] = (int*)calloc(s2_len, sizeof(int));
        dp[i] = (int*)calloc(s2_len, sizeof(int));
    }
    for (int i = 0; i < s2_len; i++) {
        col[i] = (int*)calloc(s1_len, sizeof(int));
    }

    for (int i = 0; i < s2_len; i++) {
        if (s1[0] == s2[i]) {
            dp[0][i] = 1;
            row[0][0] = 1;
        }
    }

    for (int i = 1; i < s1_len; i++) {
        if (s2[0] == s1[i]) {
            dp[i][0] = 1;
            col[0][0] = 1;
        }
    }
    for (int i = 1; i < s1_len; i++) {
        for (int j = 1; j < s2_len; j++) {
            if (s1[i] == s2[j]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                row[i][dp[i][j] - 1] = 1;
                col[j][dp[i][j] - 1] = 1;
            }
        }
    }

    for (int i = 0; i < s1_len; i++) {
        for (int j = 0; j < s2_len; j++) {
            if (dp[i][j]) {
                if (row[i][dp[i][j] - 1] && col[j][dp[i][j] - 1]) {
                    for (int dem = dp[i][j] - 1; dem >= 0; dem--) {
                        printf("%c", *(s1 + i - dem));
                    }
                    puts("");
                    row[i][dp[i][j] - 1] = 0;
                    col[j][dp[i][j] - 1] = 0;
                }
            }
        }
    }
    for (int i = 0; i < s1_len; i++) {
        for (int j = 0; j < s2_len; j++) {
            printf("%d ", dp[i][j]);
        }
        puts("");
    }

    return 0;
}