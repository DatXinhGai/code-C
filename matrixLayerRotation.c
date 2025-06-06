//
// Created by HP LAPTOP on 07/02/2025.
//
/*
Input (stdin)
2 2 3
1 1
1 1

Expected Output
1 1
1 1



Input (stdin)
5 4 7
1 2 3 4
7 8 9 10
13 14 15 16
19 20 21 22
25 26 27 28

Expected Output
28 27 26 25
22 9 15 19
16 8 21 13
10 14 20 7
4 3 2 1
*/

/*
 * SPIRIT là ánh xạ từ bảng 2D vào các layer 1D và xoay các layer đó
 * sau đó ánh xạ lại vào bảng
 * 2 thao tác chỉ ngược nhau đúng phần đẩy phần tử
 * nên dùng hàm callback(hàm lấy đối số là hàm) để tận dụng lại mã
 * so với cách xoay trực tiếp từng phần thì nhanh hơn vì
 * chỉ cần ánh xạ 1 lượt xoay từ layer vào arr
 * còn cách xoay thì phải rotate n lần với n là chu vi của layer đó
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int* layer_arr;
    int layer_length;
} LAYER;


void moveLayer(int huong[2], int* i, int* j) {
    *i += huong[0];
    *j += huong[1];
}

void ganGridVaoLayer(int* gridu, int* layeru) {
    *layeru = *gridu;
}

void ganLayerVaoGrid(int* gridu, int* layeru) {
    *gridu = *layeru;
}

void anhXa(int*** grid, LAYER** layer, int layer_max, int r, int c, void (*gan)(int* gridu, int* layeru)) {
    int huong[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    int layer_count = 0;
    int tren = 0;
    int duoi = r - 1;
    int trai = 0;
    int phai = c - 1;
    int i = 0;
    int j = 0;
    while (layer_count < layer_max) {

        int move = 0;
        int layer_cs = 0;

        while (true) {
            (*gan)(&(*grid)[i][j], &(*layer)[layer_count].layer_arr[layer_cs]);
            layer_cs++;
            if (layer_cs == (*layer)[layer_count].layer_length) {
                i++;
                layer_count++;
                tren++;
                duoi--;
                trai++;
                phai--;
                break;
            }
            if (i + huong[move][0] >= tren && i + huong[move][0] <= duoi && // vẫn di chuyển theo hướng cũ
                j + huong[move][1] >= trai && j + huong[move][1] <= phai) {
                moveLayer(huong[move], &i, &j);
            } else {    // đổi hướng
                move++;
                moveLayer(huong[move], &i, &j);
            }
        }
    }
}




void xoay(LAYER *layercs, int n) {
    int dich = n%(layercs->layer_length);
    int* sao = (int*)malloc(layercs->layer_length*sizeof(int));
    for (int i = 0; i < layercs->layer_length; i++) {
        sao[(i + dich)%layercs->layer_length] = layercs->layer_arr[i];
    }
    free(layercs->layer_arr);
    layercs->layer_arr = sao;
    sao = NULL;
}


int matrixLayerRotation() {
    int r, c, n;
    scanf("%d %d %d", &r, &c, &n);
    int** grid = (int**)malloc(r*sizeof(int*));
    for (int i = 0; i < r; i++) {
        grid[i] = (int*)malloc(c*sizeof(int));
        for (int j = 0; j < c; j++) {
            scanf("%d", &grid[i][j]);
        }
    }

    // tạo các layer
    int layer_max = (r < c)? (r/2 + r%2) : (c/2 + c%2);
    LAYER* layer = (LAYER*)malloc(layer_max*sizeof(LAYER));
    int row = r;
    int col = c;
    for (int i = 0; i < layer_max; i++) {
        if (row == 1) {
            layer[i].layer_length = col;
            layer[i].layer_arr = (int*)malloc(col*sizeof(int));
        } else if (col == 1) {
            layer[i].layer_length = row;
            layer[i].layer_arr = (int*)malloc(row*sizeof(int));
        } else {
            layer[i].layer_length = (row + col)*2 - 4;
            layer[i].layer_arr = (int*)malloc(layer[i].layer_length*sizeof(int));
        }
        row -= 2;
        col -= 2;
    }


    anhXa(&grid, &layer, layer_max, r, c, ganGridVaoLayer);

    for (int i = 0; i < layer_max; i++) {
        xoay(&layer[i], n);
    }

    anhXa(&grid, &layer, layer_max, r, c, ganLayerVaoGrid);

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            printf("%d ", grid[i][j]);
        }
        if (i != r - 1) {
            puts("");
        }
    }

    for (int i = 0; i < r; i++) {
        free(grid[i]);
        grid[i] = NULL;
    }
    free(grid);
    grid = NULL;

    for (int i = 0; i < layer_max; i++) {
        free(layer[i].layer_arr);
        layer[i].layer_arr = NULL;
    }
    free(layer);
    layer = NULL;


    return 0;

}