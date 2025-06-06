
//
// Created by HP LAPTOP on 31/01/2025.
//
/*
INPUT
4 4
1 7 8 4
4 6 2 8
3 2 5 7
1 5 6 3

OUTPUT
8


INPUT
4 3
 1 -1  3
 2  1 -1
-1  2 -1
 3 -1 -1

OUTPUT
1
*/
// NẾU DÙNG MẢNG CON TRỎ THAY CHO MEMCPY THÌ SAO?
/*
     *  tạo mảng temp lưu các chỉ số xung quanh của cặp
     *  chỉ thêm các số >= 0 tương ứng các path
     *  mảng này cần các chỉ số > 0
     *  tạo thêm mảng count nữa, quan tâm chiSo mà count[chiSo] = 2, break nhận chiSo gán vào temp[0]
     *  chạy thêm vòng nữa tìm những thằng mà count[i] = 1, thêm vào temp và đếm,
     *  chạy đến đếm để add PATH;
     */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int hang;
    int cot;
} TOADO;

typedef struct {
    TOADO toado[2];
    int viTri;
    bool noi;
} CAP;

typedef struct {
    TOADO* toado;
    // có thể dùng TOADO** toado để không phải memcpy mà dùng con trỏ để thay đổi giá trị thôi
    // đỡ tốn thời gian và không gian
    // bản chất là mảng con trỏ
    int toaDoCount;
    int toaDoMax;
} PATH;

void xetXungQuanhPkms(int *luu, int gt, int chan) {
    if (gt >= 0 && gt < chan) {
        luu[gt] += 1;
    }
}



int Pokemons() {
    int hang, cot;
    scanf("%d%d", &hang, &cot);
    hang += 2;
    cot += 2;
    int max = 0;

    // các hàng, cột bên ngoài coi như là path[0]
    int **bang = (int**)malloc(hang*sizeof(int*));
    for (int i = 0; i < hang; i++) {
        bang[i] = (int*)calloc(cot, sizeof(int));
    }
    // nhập vào và tìm max
    for (int i = 1; i < hang - 1; i++) {
        for (int j = 1; j < cot - 1; j++) {
            scanf("%d", &bang[i][j]);
            if (bang[i][j] > max) {
                max = bang[i][j];
            }
        }
    }

    // tạo cặp, số cặp là max, cặp bắt đầu từ chỉ số 1;
    CAP* cap = (CAP*)malloc((max + 1)*sizeof(CAP));
    for (int i = 1; i < hang - 1; i++) {
        for (int j = 1; j < cot - 1; j++) {
            if (bang[i][j] > 0) {
                cap[bang[i][j]].viTri = 0;
                cap[bang[i][j]].noi = false;
            }
        }
    }
    for (int i = 1; i < hang - 1; i++) {
        for (int j = 1; j < cot - 1; j++) {
            if (bang[i][j] > 0) {
                cap[bang[i][j]].toado[cap[bang[i][j]].viTri].hang = i;
                cap[bang[i][j]].toado[cap[bang[i][j]].viTri].cot = j;
                cap[bang[i][j]].viTri++;
            }
        }
    }

    // đảo hết lại để cho chỉ số của path bắt đầu từ 0, -1 thành max + 1
    for (int i = 1; i < hang - 1; i++) {
        for (int j = 1; j < cot - 1; j++) {
            if (bang[i][j] > 0) {
                bang[i][j] *= -1;
            } else if (bang[i][j] == -1) {
                bang[i][j] = max + 1;   // có tối đa max + 1 path bắt đầu từ 0
            }
        }
    }

    // khởi tạo mảng PATH, tạo path[0]
    int path_count = 1;
    PATH* path = (PATH*)malloc((max + 1)*sizeof(PATH));
    path[0].toaDoCount = 0;
    path[0].toaDoMax = 32;
    while ((hang + cot)*2 - 4 >= path[0].toaDoMax) {
        path[0].toaDoMax <<= 1;
    }
    path[0].toado = (TOADO*)malloc(path[0].toaDoMax*sizeof(TOADO));
    for (int i = 0; i < hang; i++) {
        path[0].toado[path[0].toaDoCount].hang = i;
        path[0].toado[path[0].toaDoCount++].cot = 0;
        path[0].toado[path[0].toaDoCount].hang = i;
        path[0].toado[path[0].toaDoCount++].cot = cot - 1;
    }
    for (int j = 1; j < cot - 1; j++) {
        path[0].toado[path[0].toaDoCount].hang = 0;
        path[0].toado[path[0].toaDoCount++].cot = j;
        path[0].toado[path[0].toaDoCount].hang = hang - 1;
        path[0].toado[path[0].toaDoCount++].cot = j;
    }




    int dem = 0;
    while (true) {
        bool noi = false;
        for (int i = 1; i <= max; i++) {
            if (cap[i].noi) {
                continue;
            }

            int temp[8];
            int temp_size = 0;
            int *luu0 = (int*)calloc(path_count, sizeof(int));
            int *luu1 = (int*)calloc(path_count, sizeof(int));

            int row0 = cap[i].toado[0].hang;
            int col0 = cap[i].toado[0].cot;
            int row1 = cap[i].toado[1].hang;
            int col1 = cap[i].toado[1].cot;

            xetXungQuanhPkms(luu0, bang[row0 - 1][col0], path_count);
            xetXungQuanhPkms(luu0, bang[row0 + 1][col0], path_count);
            xetXungQuanhPkms(luu0, bang[row0][col0 - 1], path_count);
            xetXungQuanhPkms(luu0, bang[row0][col0 + 1], path_count);

            xetXungQuanhPkms(luu1, bang[row1 - 1][col1], path_count);
            xetXungQuanhPkms(luu1, bang[row1 + 1][col1], path_count);
            xetXungQuanhPkms(luu1, bang[row1][col1 - 1], path_count);
            xetXungQuanhPkms(luu1, bang[row1][col1 + 1], path_count);

            int cs = -1;
            for (int j = 0; j < path_count; j++) {
                if (luu0[j] >= 1 && luu1[j] >= 1) {
                    cs = j;     // tìm path chung có chỉ số min
                    temp[temp_size++] = j;
                    break;
                }
            }

            for (int j = 0; j < path_count; j++) {
                if ((luu0[j] != 0 || luu1[j] != 0) && j != cs) {
                    temp[temp_size++] = j;  // tìm các path còn lại để hợp nhất path
                }
            }

            if (cs == -1) {
                // 2 thằng cạnh nhau thì thêm path mới
                if ((abs(row0 - row1) == 1 && col0 - col1 == 0) || (abs(col0 - col1) == 1 && row0 - row1 == 0)) {
                    bang[row0][col0] = path_count;
                    bang[row1][col1] = path_count;
                    path[path_count].toaDoMax = 32;
                    path[path_count].toado = (TOADO*)malloc(path[path_count].toaDoMax*sizeof(TOADO));
                    path[path_count].toaDoCount = 2;
                    path[path_count].toado[0].hang = row0;
                    path[path_count].toado[0].cot = col0;
                    path[path_count].toado[1].hang = row1;
                    path[path_count].toado[1].cot = col1;
                    path_count++;
                    cap[i].noi = true;
                    noi = true;
                    dem++;
                }
            } else {
                // chung path rồi đây
                // cho 2 thằng đang xét vào path[cs]
                bang[row0][col0] = cs;
                bang[row1][col1] = cs;
                path[cs].toado[path[cs].toaDoCount].hang = row0;
                path[cs].toado[path[cs].toaDoCount++].cot = col0;
                path[cs].toado[path[cs].toaDoCount].hang = row1;
                path[cs].toado[path[cs].toaDoCount++].cot = col1;
                cap[i].noi = true;
                noi = true;
                dem++;

                // cộng size cho thằng bé nhất và realloc
                for (int j = 1; j < temp_size; j++) {
                    while (path[cs].toaDoCount + path[temp[j]].toaDoCount >= path[cs].toaDoMax) {
                        path[cs].toaDoMax <<= 1;
                        path[cs].toado = (TOADO*)realloc(path[cs].toado, path[cs].toaDoMax*sizeof(TOADO));
                    }
                    // bắt đầu memcpy
                    memcpy(path[cs].toado + path[cs].toaDoCount, path[temp[j]].toado, path[temp[j]].toaDoCount*sizeof(TOADO));
                    for (int k = 0; k < path[temp[j]].toaDoCount; k++) {
                        bang[path[temp[j]].toado[k].hang][path[temp[j]].toado[k].cot] = cs;
                    }
                    path[cs].toaDoCount += path[temp[j]].toaDoCount;
                    path[temp[j]].toaDoCount = 0;
                }
            }
        }
        if (!noi || dem == max) {
            break;
        }
    }

    for (int i = 0; i < hang; i++) {
        for (int j = 0; j < cot; j++) {
            printf("%4d ", bang[i][j]);
        }
        puts("");
    }

    printf("%d", dem);

    for (int i = 0; i < hang; i++) {
        free(bang[i]);
        bang[i] = NULL;
    }
    free(bang);
    bang = NULL;


    free(cap);
    cap = NULL;

    free(path);
    path = NULL;


    return 0;
}