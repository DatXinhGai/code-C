#include "LocPhanTu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "thaoTacXau.h"

/*
char* readline() {
    long long data_length = 0;
    long long alloc_length = 1024;

    char *data = (char*)malloc(alloc_length);
    if (!data) {
        fprintf(stderr, "Allocated unsuccessfully");
        exit(EXIT_FAILURE);
    }

    while (true) {
        char *cursor = data + data_length;  // vị trí con chuột

        char *line = fgets(cursor, alloc_length - data_length, stdin);  // tự động lấy dữ liệu từ buffer
        if (!line) {
            cursor = NULL;
            break;
        }

        data_length += strlen(line);
        if (data_length < alloc_length || data[data_length - 1] == '\n') {
            break;
        }

        long long new_length = alloc_length << 1;   // nhân 2 alloc_length
        char* temp = (char*)realloc(data, new_length);
        if (!temp) {
            cursor = NULL;
            line = NULL;
            break;
        }
        data = temp;
        alloc_length = new_length;

    }


    while (data_length > 0 && (data[data_length - 1] < '0' || data[data_length - 1] > '9')) {
        data[data_length - 1] = '\0';
        data_length--;
    }
    data = (char*)realloc(data, data_length + 1);
    return data;
}
*/

void inRa(long long *so, int dem) {
    for (int i = 0; i < dem; i++) {
        printf("%lld ", so[i]);
    }
}

// nhiều vòng lặp lồng nhau, nhưng không bị đổi thứ tự
void dichPhanTu(long long** so, int* dem) {
    for (int i = 0; i < *dem; i++) {

        for (int j = i + 1; j < *dem; j++) {
            if ((*(*so + j)^*(*so + i)) == 0) {

                for (int k = j + 1; k < *dem; k++) {
                    (*so)[k - 1] = (*so)[k];

                }
                (*dem)--;   // phải có ngoặc
                j--;        // bắt buộc để xét lại vị trí đó
            }
        }
    }
    *so = (long long*)realloc(*so, *dem*sizeof(long long));

    printf("Lọc theo cách dịch phần tử:\n");
    inRa(*so, *dem);
}



// có thể bị mất thứ tự
void doiChoCuoi(long long** so, int* dem) {
    for (int i = 0; i < *dem; i++) {
        for (int j = i + 1; j < *dem; j++) {
            if ((*(*so + j)&*(*so + i)) == *(*so + j)) {
                // == có độ ưu tiên cao hơn & và ^
                // && có độ ưu tiên cao hơn ||
                *(*so + j) += *(*so + *dem - 1);
                *(*so + *dem -  1) = *(*so + j) - *(*so + *dem - 1);
                *(*so + j) -= *(*so + *dem - 1);
                (*dem)--;
                j--;
            }
        }
    }
    *so = (long long*)realloc(*so, *dem*sizeof(long long));

    printf("Lọc theo cách đổi chỗ với số cuối: \n");
    inRa(*so, *dem);
}



// tốn bộ nhớ cho mảng phụ mà vẫn 2 vòng for
void mangPhuUnique(long long *so, int dem) {
    int unique_len = 1024;
    int *unique = (int*)malloc(unique_len*sizeof(int));
    int dai = 1;
    unique[0] = so[0];
    bool dung;
    for (int i = 1; i < dem; i++) {
        dung = true;
        for (int j = 0; j < dai; j++) {
            if (unique[j]^so[i] == 0) {
                dung = false;
                break;
            }
        }
        if (dung) {
            unique[dai++] = so[i];
            if (dai == unique_len - 1) {
                unique_len <<= 1;
                long long *temp;
                temp = (long long*)realloc(unique, unique_len*sizeof(long long));
                if (!temp) {
                    fprintf(stderr, "Tràn ngập bộ nhớ nhớ nhớ nhớ e!\n");
                    exit(EXIT_FAILURE);
                }
                unique = temp;
            }
        }
    }
    printf("Lọc theo cách dùng mảng phụ:\n");
    inRa(unique, dai);

    free(unique);
    unique = NULL;
}


// dùng cực tốt khi min và max không quá lớn (độ phức tạp o(n))
// thêm biến chỉ số
void mangPhuChiSo(long long *so, int dem) {
    long long min = so[0];
    long long max = so[0];
    for (int i = 0; i < dem; i++) {
        max = (so[i] > max)? so[i] : max;
        min = (so[i] < min)? so[i] : min;
    }

    long long *phu = (long long*)calloc((max - min + 1), sizeof(long long));
    for (int i = 0; i < dem; i++) {
        phu[so[i] - min] += 1;
    }

    printf("Lọc theo cách dùng mảng chỉ số:\n");
    for (int i = 0; i < dem; i++) {
        if (phu[so[i] - min] == 1) {
            printf("%lld ", so[i]);
        }
    }

    free(phu);
    phu = NULL;
}


void locPhanTu() {
    printf("Nhập các số cách nhau bởi khoảng trắng, xuống dòng để kết thúc:\n");
    char* input = ltrim(rtrim(readline()));
    char* n_ptr = input;
    char* end_ptr;

    long long *so = (long long*)malloc(strlen(input)/2*sizeof(long long));
    int dem = 0;
    do {
        while (*n_ptr == ' ' || *n_ptr == 't') {
            n_ptr++;    // Bỏ qua khoảng trắng vì dấu cách ở cuối ức chế vc
        }
        so[dem++] = strtol(n_ptr, &end_ptr, 10);    // thêm cách trống vào cuối để tránh lỗi
        if (end_ptr == n_ptr) {
            break;
        }
        n_ptr = end_ptr;
    } while (*n_ptr != '\0');
    so = (long long*)realloc(so, dem*sizeof(long long));

    //dichPhanTu(&so, &dem);

    //doiChoCuoi(&so, &dem);

    //mangPhuUnique(so, dem);

    mangPhuChiSo(so, dem);


    free(input);
    input = NULL;

    free(so);
    so = NULL;
}