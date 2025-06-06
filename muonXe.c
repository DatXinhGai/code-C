//
// Created by HP LAPTOP on 01/02/2025.
//
/*
 *INPUT
08:15 09:15 5
08:30 11:00 2
12:30 14:00 7
17:30 19:00 2
20:00 21:00 5
$

OUTPUT
4

INPUT
08:15 09:15 5
08:30 11:00 2
12:30 14:00 7
13:00 15:00 2
17:30 19:00 2
20:00 21:00 5
$

OUTPUT
5
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/*
INPUT
08:15 09:15 5
08:30 11:00 2
12:30 14:00 7
17:30 19:00 2
20:00 21:00 5
$

OUTPUT
4

INPUT
08:15 09:15 5
08:30 11:00 2
12:30 14:00 7
13:00 15:00 2
17:30 19:00 2
20:00 21:00 5
$

OUTPUT
5
*/


typedef struct {
    char start[6];
    char end[6];
    int nguoi;
    bool tra;
} MUON;

int muonXe(void) {
    int muon_max = 32;
    int muon_count = 0;
    MUON* muon = (MUON*)malloc(muon_max*sizeof(MUON));
    while (true) {
        char temp[1024];
        fgets(temp, 1023, stdin);
        if (temp[0] == '$') {
            break;
        }
        sscanf(temp, "%s %s %d",
            muon[muon_count].start,
            muon[muon_count].end,
            &muon[muon_count].nguoi);
        muon[muon_count].tra = false;
        muon_count++;
        if (muon_count >= muon_max) {
            muon_max <<= 1;
            muon = (MUON*)realloc(muon, muon_max*sizeof(MUON));
        }
    }
    if (muon_count < muon_max) {
        muon = (MUON*)realloc(muon, muon_count*sizeof(MUON));
    }

    int tong_xe = 0;
    int cur_xe = 0;
    /*
     * sau mỗi lần thêm log mới thì xét từ đầu xem có xe nào trả không
     * bằng cách so sánh thời gian mượn và thời gian các log kia trả
     * nếu trả thì thêm vào cur_xe
     */

    for (int i = 0; i < muon_count; i++) {
        for (int j = 0; j < i; j++) {
            if (!muon[j].tra) {
                if (strcmp(muon[j].end, muon[i].start) <= 0) {
                    muon[j].tra = true;
                    cur_xe += (int)ceil(muon[j].nguoi/2.0);
                }
            }
        }
        int xe_can = (int)ceil(muon[i].nguoi/2.0);
        if (cur_xe >= xe_can) {
            cur_xe -= xe_can;
        } else {
            tong_xe += xe_can - cur_xe;
            cur_xe = 0;
        }
    }
    printf("%d", tong_xe);

    free(muon);
    muon = NULL;

    return 0;
}