//
// Created by HP LAPTOP on 10/01/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thaoTacXau.h"


typedef struct {
    char spID[10];
    char spten[100];
    double gia;
    int soLuong;
} Sp;

typedef struct {
    char maID[10];
    double rate;
    double giaMin;
    double giamMax;
} Ma;

typedef struct {
    int donID;
    int soSp;
    Sp *spmua;
    Ma *mamua;
    double tien;
    double check_out;
} Don;


// hàm nhập vào sản phẩm
// nếu có thì cộng thêm số lượng còn không thì lưu thêm sản phẩm mới
// dùng spMax không đủ thì cấp phát thêm
void nhapSp(Sp** sp, int* spNum) {
    static int spMax = 1024;
    printf("Nhập ID, tên, giá(VND) và số lượng trên cùng 1 dòng:\n");
    while (1) {
        char* temp = malloc(1024);
        fgets(temp, 1000, stdin);
        temp[strcspn(temp, "\r\n")] = '\0';
        if (strlen(temp) == 0) {
            break;
        }
        char tamID[10];
        char tamten[100];
        double tamgia;
        int tamsoLuong;
        sscanf(temp,"%s %s %lf %d", tamID, tamten, &tamgia, &tamsoLuong);
        int them = 1;
        for (int i = 0; i < *spNum; i++) {
            if (!strcmp((*sp)[i].spID, tamID)) {
                them = 0;
                (*sp)[i].soLuong += tamsoLuong;
                break;
            }
        }
        if (them) {
            strcpy((*sp)[*spNum].spID, tamID);
            strcpy((*sp)[*spNum].spten, tamten);
            (*sp)[*spNum].gia = tamgia;
            (*sp)[*spNum].soLuong = tamsoLuong;
            (*spNum)++;
            if (spMax <= *spNum) {
                spMax <<= 1;
                *sp = (Sp*)realloc(*sp, spMax*sizeof(Sp));
            }
        }
        free(temp);
    }
}

void nhapMa(Ma** ma, int* maNum) {
    static int maMax = 1024;
    printf("Nhập ID, tỉ lệ giảm(%), giá tối thiểu(VND) và mức giảm tối đa(VND):\n");
    while (1) {
        char* temp = malloc(1000);
        fgets(temp, 999,stdin);
        chuanHoa_contro(temp);
        temp[strcspn(temp, "\r\n")] = '\0';
        if (strlen(temp) == 0) {
            break;
        }
        sscanf(temp,"%s %lf %lf %lf", (*ma)[*maNum].maID, &(*ma)[*maNum].rate, &(*ma)[*maNum].giaMin, &(*ma)[*maNum].giamMax);
        (*maNum)++;
        if (maMax <= *maNum) {
            maMax <<= 1;
            *ma = (Ma*)realloc(*ma, maMax*sizeof(Ma));
        }
    }
}

void inSp(Sp* sp, int spNum) {
    if (spNum) {
        printf("Danh sách sản phẩm hiện có:\n");
        for (int i = 0; i < spNum; i++) {
            printf("---------------------------------------------\n");
            printf("ID:  %s\n", sp[i].spID);
            printf("Tên: %s\n", sp[i].spten);
            printf("Giá: %10.03lf VND         Số lượng: %6d\n", sp[i].gia, sp[i].soLuong);
        }
    } else {
        printf("Còn cái nịt\n");
    }
    printf("*********************************************\n");
    printf("\n");
}



void inMa(Ma* ma, int maNum) {
    if (maNum) {
        puts("Các mã giảm giá hiện có: \n");
        for (int i = 0; i < maNum; i++) {
            printf("---------------------------------------------\n");
            printf("ID: %s\n", ma[i].maID);
            printf("Tỉ lệ giảm: %10.2lf\%\n", ma[i].rate);
            printf("Giá tối thiểu: %10.2lf VND\n", ma[i].giaMin);
            printf("Giảm tối đa: %10.2f VND\n", ma[i].giamMax);
        }
    } else {
        printf("Hết rồi đi săn mã đi:))\n");
    }
    printf("*********************************************\n");
    printf("\n");
}

//void muaHang(Sp* sp, int spNum, Ma* ma, int maNum, &maDon) {

//}


void muaBan() {
    Sp* sp = malloc(1024*sizeof(Sp));
    int spNum = 0;
    Ma* ma = malloc(1024*sizeof(Ma));
    int maNum = 0;
    int spMua = 0;


    int maDon = 1;
    nhapSp(&sp, &spNum);
    inSp(sp, spNum);
    nhapMa(&ma, &maNum);
    inMa(ma, maNum);


}