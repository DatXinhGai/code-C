//
// Created by HP LAPTOP on 28/02/2025.
//
#include <stdio.h>
#include <string.h>

void taokBit1(char* s, int length, int count_1, int dai, int dem) {
    if (dai == length ) {
        if (dem == count_1) {
            puts(s);
        }
        return ;
    }

    for (int i = 0; i < 2; i++) {
        s[dai] = i + '0';
        if (s[dai] == '1') {
            dem++;
            if (dem > count_1) {
                return ;
            }
        }
        taokBit1(s, length, count_1, dai + 1, dem);
    }
}

int taoXauNhiPhan() {
    int length, count_1;
    scanf("%d%d", &length, &count_1);

    char s[length + 1];
    s[length] = '\0';
    int dai = 0, dem = 0;
    taokBit1(s, length, count_1, dai, dem);

}