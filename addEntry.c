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
    if (ListPtr->next == NULL) {
        return NULL;
    }

    // add null to the new end of the list
     ListPtr->next->next = NULL;
    return ListPtr->next;
}