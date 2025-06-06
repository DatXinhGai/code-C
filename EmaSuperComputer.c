//
// Created by HP LAPTOP on 07/02/2025.
//

/*
 * định lí cauchy và bất đẳng thức vào việc
 * 2 số tổng không đổi thì tích càng lớn khi hiệu 2 số càng bé
 * chia 2 trường hợp là cùng hàng hoặc cột và khác hàng, cột
 */

/*
5 6
GGGGGG
GBBBGB
GGGGGG
GGBBGB
GGGGGG
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int dai;
    int hang;
    int cot;
} CONG;

int xetXungQuanhCong(char** grid, int r, int c, int i, int j) {
    if (i >= 0 && i < r && j >= 0 && j < c && grid[i][j] == 'G') {
        return 0;
    }
    return 1;
}

void xetCong(char** grid, int r, int c, int i, int j, CONG* cong, int *dem_cong) {
    if (grid[i][j] == 'B') {
        return ;
    }
    int dai = 0;
    while (true) {
        int hople = 0;
        hople += xetXungQuanhCong(grid, r, c, i - dai - 1, j);
        hople += xetXungQuanhCong(grid, r, c, i + dai + 1, j);
        hople += xetXungQuanhCong(grid, r, c, i, j - dai - 1);
        hople += xetXungQuanhCong(grid, r, c, i, j + dai + 1);
        if (!hople) {
            dai++;
        } else {
            break;
        }
    }
    if (dai >= 1) {
        cong[*dem_cong].hang = i;
        cong[*dem_cong].cot = j;
        cong[*dem_cong].dai = dai;
        (*dem_cong)++;
    }
}

int maxCong(int a, int b) {
    return (a > b)? a : b;
}

int nhan(int a, int b) {
    return (4*a + 1)*(4*b + 1);
}

int trung(int lech, CONG a, CONG b) {
    if (a.dai + b.dai < lech) {
        return nhan(a.dai, b.dai);
    }
    int giua = lech/2;
    if (lech%2 == 1) {
        giua++;
    }

    if (a.dai >= giua && b.dai >= giua) {
        return nhan(giua - 1, lech - giua);
    }
    if (a.dai >= giua) {
        return nhan(lech - 1 - b.dai, b.dai);
    }
    return nhan(a.dai, lech - 1 - a.dai);

}

int tinhS(CONG a, CONG b) {
    int lechhang = abs(a.hang - b.hang);
    int lechcot = abs(a.cot - b.cot);
    if (lechhang == 0) {
        return trung(lechcot, a, b);
    }
    if (lechcot == 0) {
        return trung(lechhang, a, b);
    }

    int max = 0;;
    int cathang = 0;
    int catcot = 0;
    int shang;
    int scot;
    if (a.dai >= lechhang && b.dai >= lechcot) {
        cathang = 1;
        shang = maxCong(nhan(lechhang - 1, b.dai), nhan(a.dai, lechcot - 1));
    }
    if (a.dai >= lechcot && b.dai >= lechhang) {
        catcot = 1;
        scot = maxCong(nhan(lechcot - 1, b.dai), nhan(a.dai, lechhang - 1));
    }

    if (!cathang && !catcot) {
        return nhan(a.dai, b.dai);
    }

    if (cathang && catcot) {
        return maxCong(nhan(lechhang - 1, lechcot), nhan(lechhang, lechcot - 1));
    }
    if (cathang) {
        return shang;
    }

    return scot;
}

int EmaSuperComputer() {
    int r, c;
    scanf("%d%d", &r, &c);
    char** grid = (char**)malloc(r*sizeof(char*));
    for (int i = 0; i < r; i++) {
        grid[i] = (char*)malloc(1024);
        scanf("%s", grid[i]);
        grid[i][strcspn(grid[i], "\r\n")] = '\0';
    }

    int dem_cong = 0;
    int max_cong = 32;

    CONG* cong = (CONG*)malloc(max_cong*sizeof(CONG));
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            xetCong(grid, r, c, i, j, cong, &dem_cong);
            if (dem_cong >= max_cong) {
                max_cong <<= 1;
                cong = (CONG*)realloc(cong, max_cong*sizeof(CONG));
            }
        }
    }

    int max = 1;
    for (int i = 0; i < dem_cong; i++) {
        if (nhan(cong[i].dai, 0) > max) {
            max = nhan(cong[i].dai, 0);
        }
    }


    for (int i = 0; i < dem_cong; i++) {
        for (int j = i + 1; j < dem_cong; j++) {
            int s = tinhS(cong[i], cong[j]);
            if (s > max) {
                max = s;
            }
        }
    }

    printf("%d", max);


    return 0;
}
