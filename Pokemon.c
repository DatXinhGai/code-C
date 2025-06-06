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
// khi dùng biến register thì bỏ được 2 bước là
// nạp biến từ RAM vào thanh ghi
// trả kết quả từ ALU về thanh ghi xong phải trả về RAM

typedef struct {
    int hang;
    int cot;
} TOADO;

typedef struct {
    TOADO** toado;
    int viTri;
    bool noi;
} CAP;

typedef struct {
    TOADO** toado;
    int toaDoCount;
    int toaDoMax;
} PATH;

void xetXungQuanhPkm(int *luu, int gt, int chan) {
    if (gt >= 0 && gt < chan) {
        luu[gt]++;
    }
}



int Pokemon() {
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
    for (register int i = 1; i < hang - 1; i++) {
        for (register int j = 1; j < cot - 1; j++) {
            scanf("%d", &bang[i][j]);
            if (bang[i][j] > max) {
                max = bang[i][j];
            }
        }
    }

    // khởi tạo mảng toado, sau đó tạo các con trỏ trỏ vào
    TOADO** toado = (TOADO**)malloc(hang*sizeof(TOADO*));
    for (register int i = 0; i < hang; i++) {
        toado[i] = (TOADO*)malloc(cot*sizeof(TOADO));
        for (register int j = 0; j < cot; j++) {
            toado[i][j].hang = i;
            toado[i][j].cot = j;
        }
    }

    // tạo cặp, số cặp là max, cặp bắt đầu từ chỉ số 1;
    CAP* cap = (CAP*)malloc((max + 1)*sizeof(CAP));
    for (register int i = 1; i < hang - 1; i++) {
        for (register int j = 1; j < cot - 1; j++) {
            if (bang[i][j] > 0) {
                cap[bang[i][j]].viTri = 0;
                cap[bang[i][j]].noi = false;
            }
        }
    }

    for (register int i = 1; i <= max; i++) {
        cap[i].toado = (TOADO**)malloc(2*sizeof(TOADO*));
    }


    for (register int i = 1; i < hang - 1; i++) {
        for (register int j = 1; j < cot - 1; j++) {
            if (bang[i][j] > 0) {
                cap[bang[i][j]].toado[cap[bang[i][j]].viTri] = &toado[i][j];
                cap[bang[i][j]].viTri++;
            }
        }
    }

    // đảo hết lại để cho chỉ số của path bắt đầu từ 0, -1 thành max + 1
    for (register int i = 1; i < hang - 1; i++) {
        for (register int j = 1; j < cot - 1; j++) {
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

    path[0].toado = (TOADO**)malloc(path[0].toaDoMax*sizeof(TOADO*));
    for (register int i = 0; i < hang; i++) {
        path[0].toado[path[0].toaDoCount++] = &toado[i][0];
        path[0].toado[path[0].toaDoCount++] = &toado[i][cot - 1];
    }
    for (register int j = 1; j < cot - 1; j++) {
        path[0].toado[path[0].toaDoCount++] = &toado[0][j];
        path[0].toado[path[0].toaDoCount++] = &toado[hang - 1][j];
    }




    int dem = 0;
    while (true) {
        bool noi = false;
        for (register int i = 1; i <= max; i++) {
            if (cap[i].noi) {
                continue;
            }

            int *luu0 = (int*)calloc(path_count, sizeof(int));
            int *luu1 = (int*)calloc(path_count, sizeof(int));

            int row0 = cap[i].toado[0]->hang;
            int col0 = cap[i].toado[0]->cot;
            int row1 = cap[i].toado[1]->hang;
            int col1 = cap[i].toado[1]->cot;

            xetXungQuanhPkm(luu0, bang[row0 - 1][col0], path_count);
            xetXungQuanhPkm(luu0, bang[row0 + 1][col0], path_count);
            xetXungQuanhPkm(luu0, bang[row0][col0 - 1], path_count);
            xetXungQuanhPkm(luu0, bang[row0][col0 + 1], path_count);

            xetXungQuanhPkm(luu1, bang[row1 - 1][col1], path_count);
            xetXungQuanhPkm(luu1, bang[row1 + 1][col1], path_count);
            xetXungQuanhPkm(luu1, bang[row1][col1 - 1], path_count);
            xetXungQuanhPkm(luu1, bang[row1][col1 + 1], path_count);

            int cs = -1;
            for (register int j = 0; j < path_count; j++) {
                if (luu0[j] >= 1 && luu1[j] >= 1) {
                    cs = j;     // tìm path chung có chỉ số min
                    break;
                }
            }

            if (cs == -1) {
                // 2 thằng cạnh nhau thì thêm path mới
                if ((abs(row0 - row1) == 1 && col0 - col1 == 0) || (abs(col0 - col1) == 1 && row0 - row1 == 0)) {
                    bang[row0][col0] = path_count;
                    bang[row1][col1] = path_count;
                    path[path_count].toaDoMax = 32;
                    path[path_count].toado = (TOADO**)malloc(path[path_count].toaDoMax*sizeof(TOADO*));
                    path[path_count].toaDoCount = 2;
                    path[path_count].toado[0] = cap[i].toado[0];
                    path[path_count].toado[1] = cap[i].toado[1];
                    path_count++;
                    cap[i].noi = true;
                    noi = true;
                    dem++;
                }
            } else {
                int temp[8];
                int temp_size = 0;
                for (register int j = 0; j < path_count; j++) {
                    if (luu0[j] >= 1 || luu1[j] >= 1) {
                        cs = j;
                        temp[temp_size++] = j;
                        break;
                    }
                }

                for (register int j = cs + 1; j < path_count; j++) {
                    if (luu0[j] != 0 || luu1[j] != 0) {
                        temp[temp_size++] = j;  // tìm các path còn lại để hợp nhất path
                    }
                }
                // chung path rồi đây
                // cho 2 thằng đang xét vào path[cs]
                bang[row0][col0] = cs;
                bang[row1][col1] = cs;
                if (path[cs].toaDoCount + 2 >= path[cs].toaDoMax) {
                    path[cs].toaDoMax <<= 1;
                    path[cs].toado = (TOADO**)realloc(path[cs].toado, path[cs].toaDoMax*sizeof(TOADO*));
                }
                path[cs].toado[path[cs].toaDoCount++] = cap[i].toado[0];
                path[cs].toado[path[cs].toaDoCount++] = cap[i].toado[1];
                cap[i].noi = true;
                noi = true;
                dem++;

                // cộng size cho thằng bé nhất và realloc
                for (register int j = 1; j < temp_size; j++) {
                    bool re = false;
                    while (path[cs].toaDoCount + path[temp[j]].toaDoCount >= path[cs].toaDoMax) {
                        path[cs].toaDoMax <<= 1;
                        re = true;
                    }
                    if (re) {
                        path[cs].toado = (TOADO**)realloc(path[cs].toado, path[cs].toaDoMax*sizeof(TOADO*));
                        if (!path[cs].toado) {
                            fprintf(stdout, "Lỗi");
                            exit(EXIT_FAILURE);
                        }
                    }

                    /*
                     dùng con trỏ thì gán ở bên dưới là xong
                    // bắt đầu memcpy
                    memcpy(path[cs].toado + path[cs].toaDoCount, path[temp[j]].toado, path[temp[j]].toaDoCount*sizeof(TOADO));
                    */

                    for (register int k = 0; k < path[temp[j]].toaDoCount; k++) {
                        path[cs].toado[path[cs].toaDoCount + k] = path[temp[j]].toado[k];
                        bang[path[temp[j]].toado[k]->hang][path[temp[j]].toado[k]->cot] = cs;
                    }
                    path[cs].toaDoCount += path[temp[j]].toaDoCount;
                    path[temp[j]].toaDoCount = 0;
                }
            }

            free(luu0);
            luu0 = NULL;
            free(luu1);
            luu1 = NULL;

        }
        if (!noi || dem == max) {

            break;
        }
    }
    for (register int i = 0; i < hang; i++) {
        for (register int j = 0; j < cot; j++) {
            printf("%2d ", bang[i][j]);
        }
        puts("");
    }
    printf("%d", dem);

    for (register int i = 0; i < hang; i++) {
        free(bang[i]);
        bang[i] = NULL;
    }
    free(bang);
    bang = NULL;

    for (register int i = 0; i < hang; i++) {
        free(toado[i]);
        toado[i] = NULL;
    }

    free(toado);
    toado = NULL;

    free(cap);
    cap = NULL;

    free(path);
    path = NULL;


    return 0;
}