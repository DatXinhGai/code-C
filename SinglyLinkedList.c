//
// Created by HP LAPTOP on 02/04/2025.
//
#include <stdio.h>
#include <stdlib.h>

// lưu ý khi thay đổi head thì phải truyền vào &list
// vì khi truyền vào hàm bằng tham trị thì con trỏ mới và cũ cùng trỏ vào vùng head
// tuy nhiên khi thay đổi head thì thực chất chỉ bản sao của list trỏ vào chỗ khác
struct SinglyLinkedListNode {
    int value;
    struct SinglyLinkedListNode* next;
};

typedef struct SinglyLinkedListNode SinglyLinkedListNode;

struct SinglyLinkedList {
    SinglyLinkedListNode* head;
    SinglyLinkedListNode* tail;
};

typedef struct SinglyLinkedList SinglyLinkedList;

SinglyLinkedList* createList() {
    SinglyLinkedList* list = (SinglyLinkedList*)malloc(sizeof(SinglyLinkedList));
    list->head = NULL;
    list->tail = NULL;

    return list;
}

SinglyLinkedListNode* createNode(int value) {
    SinglyLinkedListNode* node = (SinglyLinkedListNode*)malloc(sizeof(SinglyLinkedListNode));
    node->value = value;
    node->next = NULL;

    return node;
}

void createHead(SinglyLinkedList* list, int value) {
    SinglyLinkedListNode* newHead = createNode(value);
    list->head = newHead;
}

void insertAtHead(SinglyLinkedList* list, int value) {
    SinglyLinkedListNode* newNode = createNode(value);
    newNode->next = list->head;
    list->head = newNode;
}

void appendToList(SinglyLinkedList* list, int value) {

    SinglyLinkedListNode* head = list->head;
    if (!head) {
        createHead(list, value);
    } else {
        SinglyLinkedListNode* newNode = createNode(value);
        SinglyLinkedListNode* cur = head;
        while (cur->next != NULL) {
            cur = cur->next;
        }

        cur->next = newNode;
        newNode->next = NULL;
    }
}

void takeInputForList(SinglyLinkedList* list) {

    int n;
    scanf("%d", &n);

    if (n < 0) {
        fprintf(stderr, "Invalid Length!");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        int val;
        scanf("%d", &val);
        appendToList(list, val);
    }
}

int returnListLength(SinglyLinkedList* list) {
    int length = 0;
    SinglyLinkedListNode* cur = list->head;

    while (cur) {
        length++;
        cur = cur->next;
    }

    return length;
}

void insert(SinglyLinkedList* list, int value, int pos) {

    SinglyLinkedListNode* head = list->head;
    if (!head) {
        createHead(list, value);
    } else if (pos == 0) {
        insertAtHead(list, value);
    } else {
        SinglyLinkedListNode* newNode = createNode(value);
        SinglyLinkedListNode* cur = head;
        SinglyLinkedListNode* prev = cur;
        while (pos > 0) {
            prev = cur;
            cur = cur->next;
            pos--;
        }

        prev->next = newNode;
        newNode->next = cur;
    }
}

void deleteNode(SinglyLinkedListNode* node) {
    free(node);
}

void pop(SinglyLinkedList* list) {
    if (list->head == NULL) {
        puts("List is already empty!");
        return ;
    }

    SinglyLinkedListNode* temp = list->head;
    list->head = list->head->next;
    deleteNode(temp);
}

// không thay đổi vị trí của head thì chỉ cần list thôi
void deleteLast(SinglyLinkedList* list) {
    if (list->head == NULL) {
        puts("List is already empty!");
    }

    SinglyLinkedListNode* cur = list->head;
    SinglyLinkedListNode* prev = cur;
    while (cur->next != NULL) {
        prev = cur;
        cur = cur->next;
    }

    deleteNode(cur);
    prev->next = NULL;
}

void printList(SinglyLinkedList* list) {
    SinglyLinkedListNode* head = list->head;
    while (head != NULL) {
        printf("%d ", head->value);
        head = head->next;
    }

    puts("");
}

void printInReverse(SinglyLinkedListNode* head) {
    if (head == NULL) {
        return ;
    }

    printInReverse(head->next);
    printf("%d ", head->value);
}

void printListInReverse(SinglyLinkedList* list) {
    SinglyLinkedListNode* head = list->head;
    printInReverse(head);
    puts("");
}

void reverseList(SinglyLinkedList* list) {
    SinglyLinkedListNode* prev = NULL;
    SinglyLinkedListNode* cur = list->head;
    SinglyLinkedListNode* next = NULL;

    while (cur) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
}

void deleteValue(SinglyLinkedList* list, int value) {
    while (list->head && list->head->value == value) {
        list->head = list->head->next;
    }

    SinglyLinkedListNode* cur = list->head;
    SinglyLinkedListNode* prev = NULL;

    while (cur) {
        prev = cur;
        cur = cur->next;

        while (cur && cur->value == value) {
            SinglyLinkedListNode* temp = cur;
            prev->next = cur->next;
            cur = prev->next;
            free(temp);
        }
    }
}

void deleteRepeated(SinglyLinkedList* list) {
    SinglyLinkedListNode* cur = list->head;

    while (cur) {
        SinglyLinkedListNode* before_next = cur;
        SinglyLinkedListNode* next = cur->next;

        while (next) {
            if (next->value == cur->value) {
                SinglyLinkedListNode* temp = next;
                before_next->next = next->next;
                next = before_next->next;
                free(temp);
            } else {
                before_next = next;
                next = next->next;
            }
        }

        cur = cur->next;
    }
}

// merge cả 2 list vào list1;
void mergeTwoSortedList(SinglyLinkedList* list1, SinglyLinkedList* list2) {

    if (list1->head->value > list2->head->value) {
        SinglyLinkedListNode* temp = list1->head;
        list1->head = list2->head;
        list2->head = temp;
    }

    SinglyLinkedListNode* au_head = list1->head;

    SinglyLinkedListNode* prev = NULL;

    while (list1->head && list2->head) {
        if (list1->head->value <= list2->head->value) {
            prev = list1->head;
            list1->head = list1->head->next;
        } else {
            SinglyLinkedListNode* list2_prev = list2->head;

            list2->head = list1->head;
            list1->head = list2_prev;
            prev->next = list1->head;
        }
    }

    if (list1->head == NULL) {
        prev->next = list2->head;
    }

    while (prev->next) {
        prev = prev->next;
    }

    list1->tail = prev;
    list1->head = au_head;

    list2->head = list2->tail = NULL;
}


void freeList(SinglyLinkedList** list) {
    if (*list) {
        SinglyLinkedListNode* cur = (*list)->head;
        SinglyLinkedListNode* temp;

        while (cur != NULL) {
            temp = cur;
            cur = cur->next;
            free(temp);
        }

        free(*list);

        *list = NULL;
    }
}

int SinglyLinkedListTest() {
    SinglyLinkedList* list1 = createList();
    takeInputForList(list1);

    SinglyLinkedList* list2 = createList();
    takeInputForList(list2);

    mergeTwoSortedList(list1, list2);

    printList(list1);
    printListInReverse(list1);

    freeList(&list1);
    return 0;
}