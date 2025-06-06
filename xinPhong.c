//
// Created by HP LAPTOP on 30/01/2025.
//
/*
INPUT
3 P031 P305 P401
08:15 09:15 P031
08:30 09:15 P305
08:45 09:30 P401
09:30 10:30 P031
$
?query 123424 09:00 10:00
?query 123424 11:00 11:30
?query 123434 11:00 11:30
$

OUTPUT
-1
P031
P305
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    char start[6];
    char end[6];
} QUERY;

typedef struct {
    char name[10];
    QUERY* query;
    int queryCount;
    int queryMax;
} ROOM;


// cách hợp lí nhất là đọc đầu vào xem có ? không thì sẽ sscanf theo 2 cách
// còn đây đang thử nghiệm xem việc truyền hàm vào hàm khác
/* SPIRIT là thằng đầu thì auto accept
 * các thằng sau thì chạy dần các query của các phòng, nếu hợp thì thêm đến cuối
 * vẫn không được thì là -1
 * chú ý khi muốn realloc phải truyền vào địa chỉ của mảng hoặc nếu
 * mảng struct chứa mảng thì phải truyền địa chỉ của mảng struct mới realloc được mảng
 */

char *readlinexinPhong() {
    int data_len = 0;
    int alloc_len = 1024;
    char* data = (char*)malloc(alloc_len);

    while (true) {
        char* cursor = data + data_len;
        char* line = fgets(cursor, alloc_len - data_len, stdin);
        if (!line) { break; }

        data_len += strlen(line);
        if (data_len < alloc_len || data[data_len - 1] == '\n') {
            break;
        }

        alloc_len <<= 1;
        char* temp = (char*)realloc(data, alloc_len);
        if (!temp) {
            fprintf(stderr, "Reallocated unsuccessfully"),
            exit(EXIT_FAILURE);
        }
        data = temp;
    }
    if (data[data_len - 1] == '\n') {
        data_len--;
    }

    data = (char*)realloc(data, data_len + 1);
    return data;

}

void readTime(ROOM** room, int roomCount) {
    while (true) {
        char temp[1024];
        fgets(temp, 1023, stdin);
        if (temp[0] == '$') {
            break;
        }


        char name[10];
        char start[6];
        char end[6];
        sscanf(temp, "%s %s %s", start, end, name);

        for (int i = 0; i < roomCount; i++) {
            if (strcmp(name, (*room)[i].name) == 0) {
                strcpy((*room)[i].query[(*room)[i].queryCount].start, start);
                strcpy((*room)[i].query[(*room)[i].queryCount].end, end);
                (*room)[i].queryCount++;


                if ((*room)[i].queryCount >= (*room)[i].queryMax) {
                    (*room)[i].queryMax <<= 1;
                    (*room)[i].query = (QUERY*)realloc((*room)[i].query, (*room)[i].queryMax*sizeof(QUERY));
                }
            }
        }
    }
    /*
    for (int i = 0; i < roomCount; i++) {
        (*room)[i].query = (QUERY*)realloc((*room)[i].query, (*room)[i].queryCount*sizeof(QUERY));
    }
    */
    // realloc là ngáo vì lúc đó size thực sự của room là querycount chứ không phải querymax, thêm vào sẽ ghi đè
    // chỉ realloc về kích thước thật khi kết thúc hết các thao tác của nó
}


void readQuery(ROOM** room, int roomCount, char*** kq, int *kq_size, int *max_size) {
    while (true) {
        char temp[1024];
        fgets(temp, 1023, stdin);
        if (temp[0] == '$') {
            break;
        }

        char start[6];
        char end[6];
        char id[10];
        sscanf(temp, "?query %s %s %s", id, start, end);
        bool hople = false;

        for (int i = 0; i < roomCount; i++) {
            bool accept = true;
            for (int j = 0; j < (*room)[i].queryCount; j++) {
                if (strcmp(start, (*room)[i].query[j].end) < 0 && strcmp(end, (*room)[i].query[j].start) > 0) {
                    accept = false;
                    break;
                }
            }

            if (accept) {
                hople = true;
                (*kq)[*kq_size] = (char*)malloc(strlen((*room)[i].name) + 1);
                strcpy((*kq)[*kq_size], (*room)[i].name);

                // thêm vào phòng i
                strcpy((*room)[i].query[(*room)[i].queryCount].start, start);
                strcpy((*room)[i].query[(*room)[i].queryCount].end, end);
                (*room)[i].queryCount++;
                if ((*room)[i].queryCount >= (*room)[i].queryMax) {
                    (*room)[i].queryMax <<= 1;
                    (*room)[i].query = (QUERY*)realloc((*room)[i].query, (*room)[i].queryMax*sizeof(QUERY));
                }

                break;
            }
        }

        if (!hople) {
            (*kq)[*kq_size] = (char*)malloc(3);
            strcpy((*kq)[*kq_size], "-1");
        }

        (*kq_size)++;
        // mở rộng nếu cần
        if (*kq_size >= *max_size) {
            *max_size <<= 1;
            *kq = (char**)realloc(*kq, *max_size*sizeof(char*));
        }

    }
    *kq = (char**)realloc(*kq, *kq_size*sizeof(char*));
}




int xinPhong() {
    char *temp = readlinexinPhong();
    char *ptr = temp;
    int roomCount;
    int dem;
    sscanf(temp, "%d%n", &roomCount, &dem);
    ptr += dem;
    while (*ptr == ' ') { ptr++; }
    ROOM *room = (ROOM*)malloc(roomCount*sizeof(ROOM));
    for (int i = 0; i < roomCount; i++) {
        sscanf(ptr, "%s", room[i].name);
        ptr += strlen(room[i].name);
        while (*ptr == ' ') { ptr++; }
        room[i].queryCount = 0;
        room[i].queryMax = 32;
        room[i].query = (QUERY*)malloc(room[i].queryMax*sizeof(QUERY));
    }

    free(temp);
    temp = NULL;
    ptr = NULL;

    int kq_size = 0;
    int max_size = 32;
    char** kq = (char**)malloc(max_size*sizeof(char*));

    readTime(&room, roomCount);

    readQuery(&room, roomCount, &kq, &kq_size, &max_size);

    for (int i = 0; i < kq_size; i++) {
        puts(kq[i]);
    }

    for (int i = 0; i < roomCount; i++) {
        free(room[i].query);
        room[i].query = NULL;
    }
    free(room);
    room = NULL;

    for (int i = 0; i < kq_size; i++) {
        free(kq[i]);
        kq[i] = NULL;
    }
    free(kq);
    kq = NULL;
    return 0;
}