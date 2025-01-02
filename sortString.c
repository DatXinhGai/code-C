#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void timchuoi(const char* a, const char* b, char* chuoidai, int* dai) {
    if (strlen(a) > strlen(b)) {
        *dai = strlen(b);
    } else {
        *dai = strlen(a);
        *chuoidai = 'b';
    }
}

int lexicographic_sort(const char* a, const char* b) {
    char chuoidai = 'a';
    int dai;
    timchuoi(a, b, &chuoidai, &dai);
    for (int i = 0; i < dai; i++) {
        if (a[i] > b[i]) {
            return 1;
        } else if (a[i] < b[i]) {
            return -1;
        }
    }
    if (chuoidai == 'a') {
        return 1;
    }
    return -1;
}

int lexicographic_sort_reverse(const char* a, const char* b) {
    char chuoidai = 'a';
    int dai;
    timchuoi(a, b, &chuoidai, &dai);
    for (int i = 0; i < dai; i++) {
        if (a[i] > b[i]) {
            return -1;
        } else if (a[i] < b[i]) {
            return 1;
        }
    }
    if (chuoidai == 'a') {
        return -1;
    }
    return 1;
}


int sort_by_number_of_distinct_characters(const char* a, const char* b, int* unique) {
    int *kta = (int*)calloc(26, sizeof(int));
    int *ktb = (int*)calloc(26, sizeof(int));
    int *ktc = (int*)calloc(26, sizeof(int));
    int dema = 0;
    int demb = 0;
    for (int i = 0; i < strlen(a); i++) {
        if (kta[a[i] - 'a'] == 0) {
            kta[a[i] - 'a'] = 1;
        }
    }
    for (int i = 0; i < strlen(b); i++) {
        if (ktb[b[i] - 'a'] == 0) {
            ktb[b[i] - 'a'] = 1;
        }
    }
    for (int i = 0; i < 26; i++) {
        if (kta[i] == 1 && ktb[i] == 1) {
            ktc[i] = 1;
        }
    }
    for (int i = 0; i < 26; i++) {
        if (kta[i] == 1 && ktc[i] == 0) {
            dema++;
        }
        if (ktb[i] == 1 && ktc[i] == 0) {
            demb++;
        }
    }
    free(kta);
    free(ktb);
    free(ktc);
    kta = NULL;
    ktb = NULL;
    ktc = NULL;


    if (dema < demb) {
        return -1;
    } else if (demb < dema) {
        return 1;
    } else {
        return lexicographic_sort(a, b);
    }
}


int sort_by_length(const char* a, const char* b) {
    if (strlen(a) > strlen(b)) {
        return 1;
    } else if (strlen(a) < strlen(b)) {
        return -1;
    }
    return lexicographic_sort(a, b);
}

// C1:
/*
void string_sort(char** arr, const int len, int (*cmp_func)(const char* a, const char* b)) {
    // cách khác là dùng 1 mảng lưu chỉ số, mảng nữa lưu thứ tự xuất hiện,
    // nhưng dùng strcpy thì có thể sẽ ghi đè ra ngoài vùng nhớ, gây lỗi nếu chuỗi đích ngắn hơn nguồn

    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {
            if (cmp_func(arr[i], arr[j]) > 0) { // bubble sort
                char* temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}
*/


// C2
void string_sort(char** arr,const int len,int (*cmp_func)(const char* a, const char* b))
{

    int *hang = (int*)calloc(len, sizeof(int));
    int *inRa = (int*)malloc(len*sizeof(int));
    char **sao = (char**)malloc(len*sizeof(char*));

    for (int i = 0; i < len; i++) {
        sao[i] = malloc(2500*sizeof(char));
    }


    for (int i = 0; i < len; i++) {
        for (int j = i + 1; j < len; j++) {
            if (((cmp_func)(arr[i], arr[j])^1) == 0) {
                hang[i] += 1;
            } else {
                hang[j] += 1;
            }
        }
    }

    for (int i = 0; i < len; i++) {
        inRa[hang[i]] = i;
    }
    for (int i = 0; i < len; i++) {
        strcpy(sao[i], arr[inRa[i]]);
        ;
    }
    for (int i = 0; i < len; i++) {
        arr[i] = sao[i];
    }
    arr = sao;

    free(hang);
    hang = NULL;
    free(inRa);
    inRa = NULL;
}




void sortString()
{
    int n;
    scanf("%d", &n);

    char** arr;
	arr = (char**)malloc(n * sizeof(char*));

    for(int i = 0; i < n; i++){
        *(arr + i) = malloc(1024 * sizeof(char));
        scanf("%s", *(arr + i));
        *(arr + i) = realloc(*(arr + i), strlen(*(arr + i)) + 1);
    }

    string_sort(arr, n, lexicographic_sort);
    for(int i = 0; i < n; i++)
        printf("%s\n", arr[i]);
    printf("\n");

    string_sort(arr, n, lexicographic_sort_reverse);
    for(int i = 0; i < n; i++)
        printf("%s\n", arr[i]);
    printf("\n");

    string_sort(arr, n, sort_by_length);
    for(int i = 0; i < n; i++)
        printf("%s\n", arr[i]);
    printf("\n");

    string_sort(arr, n, sort_by_number_of_distinct_characters);
    for(int i = 0; i < n; i++)
        printf("%s\n", arr[i]);
    printf("\n");

    for (int i = 0; i < n; i++) {
        free(arr[i]);
        arr[i] = NULL;
    }
    free(arr);
    arr = NULL;
}