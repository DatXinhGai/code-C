#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* SPIRIT là quy hoạch động
 * Tạo 1 bảng cỡ len(a) + len(b) rồi so sánh các phần tử ở các vị trí với a là hàng và b là cột
 * nếu bằng nhau thì cộng với thằng (i - 1, j - 1)
 * lưu 1 biến max luôn đỡ phải chạy lại, xử lý thđb của i = 0 và j = 0
*/

void chuoiXauConDaiNhat(void) {
    int max = 0;

    char *a = (char*)malloc(1000*sizeof(char));
    char *b = (char*)malloc(1000*sizeof(char));

    do {
        fgets(a, 1000, stdin);
    } while (strcspn(a, "\n") == 0);
    do {
        fgets(b, 1000, stdin);
    } while (strcspn(b, "\n") == 0);

    int daia = (int)strlen(a);
    int daib = (int)strlen(b);
    if (a[daia - 1] == '\n') {
        a[--daia] = '\0';

    }
    if (b[daib - 1] == '\n') {
        b[--daib] = '\0';
    }


    int **bang = (int**)malloc(daia*sizeof(int*));
    for (int i = 0; i < daia; i++) {
        *(bang + i) = (int*)malloc(daib*sizeof(int));   // có bang rồi nên không khởi tạo lại bang[i] nữa
    }
    for (int i = 0; i < daia; i++) {
        if (a[i] == b[0]) {
            bang[i][0] = 1;
            max = 1;
        } else {
            bang[i][0] = 0;
        }
    }
    for (int i = 0; i < daib; i++) {
        if (a[0] == b[i]) {
            bang[0][i] = 1;
        } else {
            bang[0][i] = 0;
        }
    }
    for (int i = 1; i < daia; i++) {
        for (int j = 1; j < daib; j++) {
            if (a[i] == b[j]) {
                bang[i][j] = 1 + bang[i - 1][j - 1];
                if (bang[i][j] > max) {
                    max = bang[i][j];
                }
            } else {
                bang[i][j] = 0;
            }
        }
    }
    printf("Chuỗi con chung dài nhất là: %d\n", max);


    for (int i = 0; i < daia; i++) {
        if (bang[i]) {
            free(bang[i]);
            bang[i] = NULL;
        }
    }
    if (bang) {
        free(bang);
        bang = NULL;
    }
    if (a) {
        free(a);
        a = NULL;
    }
    if (b) {
        free(b);
        b = NULL;
    }
}