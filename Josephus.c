//
// Created by HP LAPTOP on 27/12/2024.
// Còn cách khác đấy
// kiểm tra malloc và giữ lại nguoi khi realloc thất bại
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void hanhQuyet(char ***nguoi, int *n, int *chiSo, int k) {
    *chiSo = (*chiSo + k)%(*n);
    for (int i = *chiSo + 1; i < *n; i++) {
        strcpy((*nguoi)[i - 1], (*nguoi)[i]);
    }
    (*n)--;     // không biết sai bao nhiêu lần rồi
    char **temp = (char**)realloc(*nguoi, (*n)*sizeof(char*));
    // cùng cấp với *nguoi là được
    if (!temp) {
        fprintf(stderr, "Realloc unsucessfully");
        exit(EXIT_FAILURE);
    } else {
        *nguoi = temp;
    }
}


void Josephus() {
    int n, k;
    scanf("%d%d", &n, &k);
    getchar();
    char **nguoi = (char**)malloc(n*sizeof(char*));
    if (!nguoi) {
        fprintf(stderr, "Allocated unsucessfully!");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        nguoi[i] = (char*)malloc(50*sizeof(char));
        if (!nguoi[i]) {
            fprintf(stderr, "Allocated unsucessfully!");
            exit(EXIT_FAILURE);
        }
        do {
            fgets(nguoi[i], 50, stdin);
            if (strlen(nguoi[i]) != 0) {
                nguoi[i][strcspn(nguoi[i], "\r\n")] = '\0';
            }
        } while (strlen(nguoi[i]) == 0);
    }

    int chiSo = 0;
    while (n > 1) {
        hanhQuyet(&nguoi, &n, &chiSo, k);
    }
    printf("%s", nguoi[0]);
    free(nguoi[0]);
    nguoi[0] = NULL;
    free(nguoi);
    nguoi = NULL;

}