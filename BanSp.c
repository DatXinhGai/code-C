//
// Created by HP LAPTOP on 25/01/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
*5
COCA5
Coca cola 500ml
12000
COCA3
Coca cola 300ml
8500
PEPSI3
Pepsi 300ml
8000
TRADA
Tra da
5000
AQUA3
Nuoc suoi 300ml
5000
BILL01
COCA3 2
AQUA3 1
#
BILL02
COCA3 2
AQUA5 1
#
BILL03
TRADA 5
COCA5 1
#
$

OUTPUT
Coca cola 500ml : 1
Coca cola 300ml : 2
Tra da : 5
Nuoc suoi 300ml : 1
*/

typedef struct {
    char* maSp;
    char* tenSp;
    int price;
    int ban;
} SP;

typedef struct {
    char* maSp;
    int soLuong;
    int STT;
} DONSP;

typedef struct {
    char* maDon;
    DONSP *donSp;
    int count;
    bool valid;
} DON;



char* readlineBanSp() {
    int alloc_length = 1024;
    int data_length = 0;

    char *data = (char*)malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);
        if (!line) {
            perror("Allocated unsuccessfully");
            exit(EXIT_FAILURE);
        }

        data_length += strlen(line);

        if (data_length < alloc_length || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;
        char* temp = (char*)realloc(data, alloc_length);
        if (!temp) {
            perror("Allocated unsucessfully");
        }
        data = temp;
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
        data_length--;
    }

    data = (char*)realloc(data, data_length + 1);
    return data;
}


void nhapSp1(SP *sp, int n) {
    for (int i = 0; i < n; i++) {
        sp[i].maSp = readlineBanSp();
        sp[i].tenSp = readlineBanSp();
        sp[i].ban = 0;
        scanf("%d", &sp[i].price);
        getchar();
    }
}


void nhapSpCuaDon(DON* don, int don_index) {
    don[don_index].count = 0;
    int *dem = &don[don_index].count;
    int count_max = 1024;
    // các sp trong 1 đơn
    don[don_index].donSp = (DONSP*)malloc(count_max*sizeof(DONSP));

    // vòng lặp của sp trong đơn
    while (true) {
        char* temp = readlineBanSp();
        if (temp[0] == '#') {
            break;
        }
        don[don_index].donSp[*dem].maSp = (char*)malloc(1024*sizeof(char));

        sscanf(temp, "%s %d",
            don[don_index].donSp[*dem].maSp,
            &don[don_index].donSp[*dem].soLuong);
        //temp_xau[strcspn(temp_xau, " ")] = '\0';
        (*dem)++;

        if (don[don_index].count >= count_max) {
            count_max <<= 1;
            DONSP* tam = (DONSP*)realloc(don[don_index].donSp, count_max*sizeof(DONSP));
            if (!tam) { break; }
            don[don_index].donSp = tam;
        }

    }
    don[don_index].donSp = (DONSP*)realloc(don[don_index].donSp, don[don_index].count*sizeof(DONSP));

}


int nhapDon(DON **don) {
    int don_index = 0;                                  // số lượng đơn
    int don_max = 1024;
    // vòng lặp của đơn

    while (true) {
        (*don)[don_index].maDon = readlineBanSp();         // đơn mới
        if ((*don)[don_index].maDon[0] == '$') {
            break;
        }

        nhapSpCuaDon(*don, don_index);
        don_index++;

        if (don_index >= don_max) {
            don_max <<= 1;
            DON *temp = (DON*)realloc(*don, don_max*sizeof(DON));
            if (!temp) { break; }
            *don = temp;
        }
    }

    *don = (DON*)realloc(*don, don_index*sizeof(DON));
    return don_index;
}


void kiemTraDon(DON* don, int soDon, SP *sp, int n) {
    for (int i = 0; i < soDon; i++) {
        don[i].valid = true;
        for (int j = 0; j < don[i].count; j++) {
            bool isValid = false;
            for (int k = 0; k < n; k++) {
                if (strstr(sp[k].maSp, don[i].donSp[j].maSp)) {
                    isValid = true;
                    don[i].donSp[j].STT = k;
                    break;
                }
            }
            if (!isValid) {
                don[i].valid = false;
                break;
            }
        }
    }
}


void tinhDon(DON* don, int soDon, SP *sp) {
    for (int i = 0; i < soDon; i++) {
        if (!don[i].valid) {
            continue;
        }

        for (int j = 0; j < don[i].count; j++) {
            sp[don[i].donSp[j].STT].ban += don[i].donSp[j].soLuong;
        }
    }
}

void tinhTien(SP *sp, int n) {
    long long tien = 0;
    for (int i = 0; i < n; i++) {
        tien += sp[i].ban * sp[i].price;
    }
    printf("%lld", tien);
}

void inThanhToan(SP* sp, int n) {
    for (int i = 0; i < n; i++) {
        if (sp[i].ban > 0) {
            printf("%s : %d\n", sp[i].tenSp, sp[i].ban);
        }
    }
}

int BanSp() {
    int n;
    scanf("%d", &n);
    getchar();
    SP *sp = (SP*)malloc(n*sizeof(SP));
    nhapSp1(sp, n);

    DON *don = (DON*)malloc(1024*sizeof(DON));  // danh sách đơn
    int soDon = nhapDon(&don);

    kiemTraDon(don, soDon, sp, n);
    tinhDon(don, soDon, sp);

    tinhTien(sp, n);
    //inThanhToan(sp, n);

    free(sp);
    sp = NULL;

    for (int i = 0; i < soDon; i++) {
        free(don[i].donSp);
        don[i].donSp = NULL;
    }

    free(don);
    don = NULL;



    return 0;
}