//
// Created by HP LAPTOP on 30/04/2025.
//
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MAX_LENGTH 1000000

typedef struct {
    int operation_type;
    char* operation_text;
} OPERATIONS;

typedef struct {
    OPERATIONS* operations;
    int operation_count;
} OPS_MANAGER;

typedef struct {
    char* text;
    int max_length;
} TEXT_STR;

void update(OPS_MANAGER* ops_manager, int operation_type, int text_length, char* text) {
    OPERATIONS** cur_ops = &(ops_manager->operations);
    int* cur_count = &(ops_manager->operation_count);

    (*cur_ops)[*cur_count].operation_type = operation_type;
    (*cur_ops)[*cur_count].operation_text = (char*)malloc(text_length + 1);
    strcpy((*cur_ops)[*cur_count].operation_text, text);
    (*cur_count)++;
}

void append(TEXT_STR* text_str, OPS_MANAGER* ops_manager, char* text, int add_to_ops) {

    int text_length = strlen(text);
    int suf_length = strlen(text_str->text) + text_length + 1;

    int expand = 0;
    while (suf_length >= text_str->max_length) {
        text_str->max_length <<= 1;
        expand = 1;
    }

    if (expand) {
        text_str->text = (char*)realloc(text_str->text, text_str->max_length);
    }

    strcat(text_str->text, text);


    if (add_to_ops) {
        update(ops_manager, 1, text_length, text);
    }


}

void del(TEXT_STR* text_str, OPS_MANAGER* ops_manager, int delete_count, int add_to_ops) {
    int text_str_length = strlen(text_str->text);

    char temp[delete_count + 1];
    int last_pos = text_str_length - delete_count;

    memcpy(temp, text_str->text + last_pos, delete_count + 1);
    text_str->text[last_pos] = '\0';


    if (add_to_ops) {
        update(ops_manager, 2, delete_count, temp);
    }

}

void print(TEXT_STR text_str, int pos) {
    printf("%c\n", text_str.text[pos - 1]);
}

void undo(TEXT_STR* text_str, OPS_MANAGER* ops_manager) {

    OPERATIONS cur_ops = ops_manager->operations[ops_manager->operation_count - 1];
    int type = cur_ops.operation_type;

    char* text = cur_ops.operation_text;

    if (type == 1) {

        int delete_count = strlen(text);
        del(text_str, ops_manager, delete_count, 0);

    } else {
        append(text_str, ops_manager, text, 0);
    }

    ops_manager->operation_count--;

}



int textEditor() {

    char luu[1024];
    fgets(luu, 1023, stdin);
    int n = strtol(luu, NULL, 10);

    // initialize the res string and it's length in case of expand
    TEXT_STR text_str;
    text_str.max_length = 1024;
    text_str.text = (char*)malloc(text_str.max_length);
    text_str.text[0] = '\0';

    // initialize a stack to save the completed operation

    OPS_MANAGER ops_manager;
    ops_manager.operations = (OPERATIONS*)malloc(n*sizeof(OPERATIONS));;
    ops_manager.operation_count = 0;

    for (int i = 0; i < n; i++) {
        char temp[1024];
        fgets(temp, 1023, stdin);
        int operation_type;

        sscanf(temp, "%d", &operation_type);

        switch (operation_type) {
            case 1 :
            {
                char operation_text[MAX_LENGTH];
                sscanf(temp + 2, "%s", operation_text);
                operation_text[strcspn(operation_text, "\r\n")] = '\0';
                append(&text_str, &ops_manager, operation_text, 1);
                break;
            }

            case 2 :
            {
                int delete_count;
                sscanf(temp + 2, "%d", &delete_count);
                del(&text_str, &ops_manager, delete_count, 1);
                break;
            }

            case 3 :
            {
                int pos;
                sscanf(temp + 2, "%d", &pos);
                print(text_str, pos);
                break;
            }

            case 4 :
            {
                undo(&text_str, &ops_manager);
            }
        }
    }


    free(text_str.text);

    for (int i = 0; i < ops_manager.operation_count; i++) {
        free(ops_manager.operations[i].operation_text);
    }

    free(ops_manager.operations);

    return 0;
}
