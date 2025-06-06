/*
2024-06-18 08:30:15 123456789012 M
2024-06-18 09:30:15 123456789010 M
2024-06-18 11:30:15 123456789012 T
2024-06-18 14:30:05 123456789012 M
2024-06-18 15:30:15 123456788012 M
2024-06-18 15:31:05 123456789022 M
2024-06-18 18:30:15 123456788012 T
$
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char ngay[11];
    char gio[9];
    char id[20];
    char status;
} NHANVIEN;

int nhapSo() {
    int dem = 0;
    int muon = 0;
    int nv_max = 1024;
    NHANVIEN *nv = (NHANVIEN*)malloc(nv_max*sizeof(NHANVIEN));

    while (true) {
        char* temp = (char*)malloc(100);
        fgets(temp, 99, stdin);
        if (temp[0] == '$') {
            break;
        }
        char ngay[11];
        char gio[11];
        char id[20];
        char status;
        sscanf(temp, "%s %s %s %c", ngay, gio, id, &status);

        bool them = true;
        for (int i = 0; i < dem; i++) {
            if (strstr(id, nv[i].id)) {
                them = false;
                strcpy(nv[i].ngay, ngay);
                strcpy(nv[i].gio, gio);
                nv[i].status = (nv[i].status == 'M')? 'T' : 'M';
                muon += (nv[i].status == 'M')? 1 : -1;
                break;
            }
        }

        if (them) {
            strcpy(nv[dem].id, id);
            strcpy(nv[dem].ngay, ngay);
            strcpy(nv[dem].gio, gio);
            nv[dem].status = 'M';
            dem++;
            muon += 1;
            if (dem == nv_max) {
                nv_max <<= 1;
                nv = (NHANVIEN*)realloc(nv, nv_max*sizeof(NHANVIEN));
            }
        }
    }


    NHANVIEN* br = (NHANVIEN*)malloc(dem*sizeof(NHANVIEN));
    int brdem = 0;
    for (int i = 0; i < dem; i++) {
        if (nv[i].status == 'M') {
            br[brdem++] = nv[i];
        }
    }

    for (int i = 0; i < brdem; i++) {
        for (int j = i + 1; j < brdem; j++) {
            if (br[i].ngay < br[j].ngay) {
                continue;
            } else if (br[i].ngay > br[j].ngay || br[i].gio > br[j].gio) {
                NHANVIEN temp = br[i];
                br[i] = br[j];
                br[j] = temp;
            }
        }
    }


    printf("The number of devices currently being borrowed: %d\n", brdem);
    for (int i = 0; i < brdem; i++) {
        printf("%s %s %s\n", br[i].ngay, br[i].gio, br[i].id);
    }
}