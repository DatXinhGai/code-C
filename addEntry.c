//
// Created by HP LAPTOP on 02/01/2025.
//
#include <stdlib.h>

struct entry {
    int value;
    struct entry *next;
};

struct entry *addEntry(struct entry *ListPtr) {
    // find the end of the list
    while (ListPtr->next != NULL) { ListPtr = ListPtr->next; }

    // get storage for new entry
    ListPtr->next = (struct entry*)malloc(sizeof(struct entry));
    // add null to the new end of the list
    if (ListPtr->next->next != NULL) {    // nếu nó bằng NULL sẵn rồi thì bỏ qua bước gán ở sau
        ListPtr->next->next = NULL;    // thằng cuối phải không trỏ linh tinh
    }

    return ListPtr->next;
}