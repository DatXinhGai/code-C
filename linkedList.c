//
// Created by HP LAPTOP on 02/01/2025.
//
#include <stdio.h>
#include <stdlib.h>

// một cấu trúc phức tạp kết hợp bởi con trỏ và cấu trúc
// nên cấp phát động cho list
// con trỏ thì phải (*newNode).data hoặc newNode->data

struct list {
    int data;
    struct list* pNext;
};


struct list* createNode(int data) {
    struct list* newNode = (struct list*)malloc(sizeof(struct list));
    newNode->data = data;
    newNode->pNext = NULL;
    return newNode;
};

struct list *searchList (struct list *pHead, int key) {
    struct list* pStart = pHead;
    do {
        // or NULL;
        if (pHead->data == key) {
            return pHead;
        } else {
            pHead = pHead->pNext;
        }
    } while (pHead != pStart);
    return (struct list*)0;     // or NULL
}

void linkedList() {
    int key;
    printf("Nhập vào số cần tìm: ");
    scanf("%d", &key);
    struct list *n1 = createNode(1);
    struct list *n2 = createNode(2);
    struct list *n3 = createNode(3);
    n1->pNext = n2;
    n2->pNext = n3;
    n3->pNext = n1; // vòng

    struct list *kq = searchList(n1->pNext, key);

    if (kq != NULL) {
        printf("Key found in the list!");
    } else {
        printf("Key not found in the list");
    }

    free(n1);
    n1 = NULL;
    free(n2);
    n2 = NULL;
    free(n3);
    n3 = NULL;
}
