//
// Created by HP LAPTOP on 07/02/2025.
//
/*
 * chia làm 4 nhóm
 * bom (O)
 * chấm không tiếp xúc bom(chỉ tiếp xúc chấm) (1)
 * chấm tiếp xúc bom và chấm 1 (2)
 * chấm tiếp xúc bom và chấm 2 (3)
 * thực ra từ lần 3 và 5 thì trạng thái lặp lại theo mod 4 nhưng thế này hiểu bản chất hơn
 * bom và chấm 1 thay nhau đổi trạng thái
 * chấm 2 luôn không là bom do bị nổ liên tục bởi O và 1
 * chấm 3 sau lần đầu thì đồng bộ tức là biến thành bom
 */

/*
Input (stdin)
6 7 3
.......
...O...
....O..
.......
OO.....
OO.....

Expected Output
OOO.OOO
OO...OO
OOO...O
..OO.OO
...OOOO
...OOOO


Input (stdin)
6 7 5
.......
...O.O.
....O..
..O....
OO...OO
OO.O...

Expected Output
.......
...O.O.
...OO..
..OOOO.
OOOOOOO
OOOOOOO
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


void xetXungQuanhBom(char** grid, int r, int c, int i, int j, bool* canhBom, bool* canhCham) {
    if (i >= 0 && i < r && j >= 0 && j < c) {
        if (grid[i][j] == 'O') {
            *canhBom = true;
        } else if (grid[i][j] == '1') {
            *canhCham = true;
        }
    }
}

void xet4phia(char** grid, int r, int c, int i, int j, bool* canhBom, bool* canhCham) {
    xetXungQuanhBom(grid, r, c, i - 1, j, canhBom, canhCham);
    xetXungQuanhBom(grid, r, c, i + 1, j, canhBom, canhCham);
    xetXungQuanhBom(grid, r, c, i, j - 1, canhBom, canhCham);
    xetXungQuanhBom(grid, r, c, i, j + 1, canhBom, canhCham);
}

void xetBomberManGame(char** grid, int r, int c, int n) {
    if (n == 0 || n == 1) {
        return ;
    }

    int status = n%4;
    if (status%2 == 0) {
        for (int i = 0 ; i < r; i++) {
            memset(grid[i], 'O', c);
        }
        return ;
    }

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (grid[i][j] != 'O') {
                bool canhBom = false;
                bool canhCham = false;
                xet4phia(grid, r, c, i , j, &canhBom, &canhCham);
                if (!canhBom) {
                    grid[i][j] = '1';
                }
            }
        }
    }

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (grid[i][j] != 'O') {
                bool canhBom = false;
                bool canhCham = false;
                xet4phia(grid, r, c, i , j, &canhBom, &canhCham);
                if (canhBom && canhCham) {
                    grid[i][j] = '2';
                }
            }
        }
    }

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (grid[i][j] == '.') {
                grid[i][j] = 'O';
            }
        }
    }

    if (status == 1) {
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                if (grid[i][j] == '1') {
                    grid[i][j] = '.';
                } else if (grid[i][j] == '2') {
                    grid[i][j] = '.';
                }
            }
        }
        return ;
    }

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (grid[i][j] == 'O') {
                grid[i][j] = '.';
            } else if (grid[i][j] == '1') {
                grid[i][j] = 'O';
            } else {
                grid[i][j] = '.';
            }
        }
    }

}

int BomberManGame() {
    int r, c, n;
    scanf("%d %d %d", &r, &c, &n);

    char** arr = (char**)malloc(r*sizeof(char*));
    for (int i = 0; i < r; i++) {
        arr[i] = (char*)malloc(1024);
        scanf("%s", arr[i]);
    }
    xetBomberManGame(arr, r, c, n);

    for (int i = 0; i < r; i++) {
        puts(arr[i]);
    }

    for (int i = 0; i < r; i++) {
        free(arr[i]);
        arr[i] = NULL;
    }

    free(arr);
    arr = NULL;

    return 0;
}