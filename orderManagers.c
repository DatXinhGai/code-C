//
// Created by HP LAPTOP on 04/06/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_NAME 50
#define MAX_NOTE 100
#define MAX_TIME 100
#define MAX_SIZE 32

typedef enum STATUS {
    SERVING = 0,
    PAID = 1,
    CANCEL = -1,
} Status;

struct Dish {
    char id[MAX_NAME];
    int ordered_nums;
    int returned_nums;
    char note[MAX_NOTE];
    struct Dish* next;
};

typedef struct Dish Dish;

struct Order {
    char created_time[MAX_TIME];
    char staff_id[MAX_NAME];
    Dish* dishes;
    int dishes_count;
    int nums_count;
    int returned_dishes;
    int returned_nums;
    char updated_time[MAX_TIME];
    Status status;
    struct Order* next;
};

typedef struct Order Order;

struct Table {
    int table_id;
    Order* order;
    struct Table* next;
};

typedef struct Table Table;

char* returnTime() {
    // struct quản lí thời gian
    struct tm* ptr;
    time_t t;

    t = time(NULL);

    // lấy thời gian khu vực
    ptr = localtime(&t);

    // ghi thời gian theo định dạng
    char* str = asctime(ptr);
    //str[strcspn(str, "\r\n")] = '\0';

    return str;

}

Table* table_list;

Table* searchTable(int table_id) {
    Table* cur = table_list;

    while (cur != NULL) {
        if (cur->table_id == table_id) {
            return cur;
        }
        cur = cur->next;
    }

    return NULL;
}

// tạo bàn mới
Table* createTable(int table_id) {
    Table* table = (Table*)malloc(sizeof(Table));
    table->order = NULL;
    table->table_id = table_id;
    table->next = NULL;

    return table;
}

// thêm bàn mới vào danh sách bàn hiện có
void addTableToLast(Table* table) {

    Table* cur = table_list;

    if (cur == NULL) {
        table_list = table;
    } else {
        while (cur->next != NULL) {
            cur = cur->next;
        }

        cur->next = table;
    }
}

// tìm và trả về đơn mới nhất của bàn
Order* searchOrder(int table_id) {
    Table* cur = table_list;
    while (cur != NULL) {
        if (cur->table_id == table_id) {
            return cur->order;
        }

        cur = cur->next;
    }

    puts("Bàn chưa gọi món!");
    return NULL;
}


// tạo order và thêm vào cuối của table, nếu chưa có thì tạo mới bàn
Order* createOrder(int table_id, char staff_id[], char time[]) {

    Table* table = searchTable(table_id);
    // tạo bàn
    if (!table) {
        table = createTable(table_id);
        addTableToLast(table);
    }
    // đã có sẵn order đang được phục vụ rồi
    if (table->order && table->order->status == SERVING) {
        puts("Bàn đã được tạo order!");
        return NULL;
    }

    Order* new_order = (Order*)malloc(sizeof(Order));

    strcpy(new_order->created_time, time);
    new_order->dishes = NULL;
    new_order->dishes_count = 0;
    new_order->nums_count = 0;
    new_order->returned_dishes = 0;
    new_order->returned_nums = 0;
    new_order->status = SERVING;
    strcpy(new_order->staff_id, staff_id);

    // cập nhật danh sách order cho bàn
    new_order->next = table->order;
    table->order = new_order;

    return new_order;
}

// khi order bị hủy hoặc được thanh toán thì chuyển về cuối danh sách
void updateOrder(Table* table) {

    char* time = returnTime();
    strcpy(table->order->updated_time , time);

    Order* cur = table->order;

    if (cur->next == NULL) {
        return ;
    }

    while (cur->next != NULL) {
        cur = cur->next;
    }

    cur->next = table->order;
    table->order = table->order->next;
    cur->next->next = NULL;

}

int cancelOrder(int table_id) {
    Table* table = searchTable(table_id);

    if (!table || !table->order || table->order->status != SERVING) {
        return 0;
    }
    // đã trả món thì không hủy được nữa
    if (table->order->returned_nums != 0) {
        return 0;
    }

    table->order->dishes_count = 0;
    table->order->nums_count = 0;
    // gán order bị hủy và update đưa về cuối và không cần in ra nữa
    table->order->status = CANCEL;
    updateOrder(table);
    return 1;
}


Dish* createDish(char dish_id[], int count, char note[]) {
    Dish* dish = (Dish*)malloc(sizeof(Dish));
    strcpy(dish->id, dish_id);
    dish->ordered_nums = count;
    dish->returned_nums = 0;
    strcpy(dish->note, note);
    dish->next = NULL;

    return dish;
}

Dish* searchDish(Dish* dishes, char dish_id[]) {
    Dish* cur = dishes;

    while (cur != NULL) {
        if (strcmp(cur->id, dish_id) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

int addDish(int table_id, char staff_id[], char time[], char dish_id[], int count, char note[]) {
    Table* table = searchTable(table_id);
    if (!table) {
        table = createTable(table_id);
        addTableToLast(table);
    }


    // nếu bàn chưa có order hoặc các order trước đó đã xong hoặc hủy
    if (table->order == NULL || table->order->status == PAID || table->order->status == CANCEL) {
        Order* order = createOrder(table_id, staff_id, time);
        table->order = order;

    }

    Dish* cur = table->order->dishes;
    Dish* prev = table->order->dishes;

    bool dish_find = false;

    while (cur != NULL) {
        prev = cur;
        if (strcmp(cur->id, dish_id) == 0) {
            dish_find = true;
            break;
        }
        cur = cur->next;
    }

    // danh sách rỗng hoặc không tìm được thì tạo món mới
    if (prev == NULL || dish_find == false) {
        Dish* new_dish = createDish(dish_id, count, note);
        if (prev == NULL) {
            table->order->dishes = new_dish;
        } else {
            prev->next = new_dish;
        }

        table->order->dishes_count++;
        table->order->nums_count += count;


    } else {    // tìm được
        // món đã hoàn thành nhưng gọi thêm thì coi như là chưa hoàn thành, và khi đó dish_count không đổi
        if (cur->ordered_nums == cur->returned_nums) {
            table->order->returned_dishes--;
        }

        // cập nhật dish
        cur->ordered_nums += count;
        strcpy(cur->note, note);

        table->order->nums_count += count;
    }

    return 1;
}



int updateDish(int table_id, char staff_id[], char dish_id[], int count) {
    Table* table = searchTable(table_id);
    if (!table) {
        return 0;
    }

    Order* order = table->order;
    if (!order || order->status != SERVING) {
        return 0;
    }

    Dish* return_dish = searchDish(order->dishes, dish_id);

    if (return_dish) {
        // tính số đĩa còn cần phải phục vụ
        int serving_nums = return_dish->ordered_nums - return_dish->returned_nums;
        if (serving_nums < count) {
            return 0;
        } else {
            return_dish->returned_nums += count;

            // nếu món đã hoàn thành thì tăng số đĩa trả của order
            if (return_dish->returned_nums == return_dish->ordered_nums) {
                order->returned_dishes++;
            }

            order->returned_nums += count;

            // cập nhật nhân viên
            strcpy(order->staff_id, staff_id);

            return 1;
        }
    }


    return 0;
}


int cancelDish(int table_id, char dish_id[], char note[]) {
    Table* table = searchTable(table_id);

    if (!table || !table->order || !table->order->dishes) {
        return 0;
    }

    Dish* return_dish = searchDish(table->order->dishes, dish_id);


    if (return_dish) {
        // nếu chưa đĩa nào trả
        if (return_dish->returned_nums == 0) {
            int count = return_dish->ordered_nums;

            return_dish->ordered_nums = 0;
            strcpy(return_dish->note, note);

            table->order->dishes_count--;
            table->order->nums_count -= count;

            return 1;
        }
    }
    return 0;
}

void printToStream(char* message, FILE* fptr) {
    fputs(message, fptr);
}

void printDish(Dish* dish, FILE* fptr) {
    if (dish == NULL) {
        return ;
    }

    if (dish->returned_nums == dish->ordered_nums) {
        char message[1024] ;
        if (dish->note[0] != '\0') {
            sprintf(message,"\tMón: %-20s     Số lượng: %-6d     Ghi chú: %s\n",dish->id, dish->ordered_nums, dish->note);
        } else {
            sprintf(message, "\tMón: %-20s     Số lượng: %-6d\n", dish->id, dish->ordered_nums);
        }

        printToStream(message, stdout);
        printToStream(message, fptr);

    }

    printDish(dish->next, fptr);
}

void printOrder(Table* table,  FILE* fptr) {
    Order *order = table->order;

    if (order == NULL) {
        return ;
    }


    char* time = returnTime();
    strcpy(order->updated_time, time);
    printToStream(order->updated_time, stdout);
    printToStream(order->updated_time, fptr);

    if (order->status == SERVING) {
        order->status = PAID;

        // nếu đơn có món thì in còn không thì in đơn bị hủy
        if (order->nums_count == 0) {
            char message[1024] = "/tĐơn chưa gọi món/n";
            printToStream(message, stdout);
            printToStream(message, fptr);
        } else {
            printDish(order->dishes, fptr);

            char message[1024];
            sprintf(message, "\tSố đĩa đặt: %d\n", order->nums_count);
            printToStream(message, stdout);
            printToStream(message, fptr);

            sprintf(message, "\tSố món đặt: %d\n", order->dishes_count);
            printToStream(message, stdout);
            printToStream(message, fptr);
        }

        updateOrder(table);

        fputs("", fptr);
    } else {
        char message[1024] = "Đơn đã bị hủy hoặc đã hoàn thành!\n";
        printToStream(message, stdout);
        printToStream(message, fptr);
        return ;
    }

}

void printTable(Table* table, FILE* fptr) {
    if (table == NULL) {
        return  ;
    }

    char s[100] = "------------------------------------------";
    puts(s);
    fprintf(fptr, "%s\n", s);

    printOrder(table, fptr);

}

void createBill(int table_id, FILE* fptr) {
    Table* table = searchTable(table_id);
    printTable(table, fptr);
}

void move(char** end_ptr) {
    while (**end_ptr == ' ') {
        (*end_ptr)++;
    }
}

char** delim(char* str, int* split_index) {
    str[strcspn(str, "\r\n")] = '\0';
    *split_index = 0;
    char** split = (char**)malloc(MAX_SIZE * sizeof(char*));

    char* n_ptr = str;
    char* end_ptr = str;
    bool open = false;

    while (*n_ptr != '\0') {
        if (open) {
            if (*n_ptr == '`') {
                open = false;
                *n_ptr = '\0';
                split[(*split_index)++] = strdup(end_ptr);
                n_ptr++;
                move(&n_ptr);
                end_ptr = n_ptr;
            } else {
                n_ptr++;
            }
        } else {
            if (*n_ptr == ' ') {
                *n_ptr = '\0';
                split[(*split_index)++] = strdup(end_ptr);
                n_ptr++;
                move(&n_ptr);
                end_ptr = n_ptr;
            } else if (*n_ptr == '`') {
                open = true;
                end_ptr = ++n_ptr;
            } else {
                n_ptr++;
            }
        }
    }

    n_ptr--;
    if (*n_ptr >= '0' && *n_ptr <= '9') {
        split[(*split_index)++] = strdup(end_ptr);
    }

    return split;
}

void freeSplit(char** split, int split_size) {
    for (int i = 0; i < split_size; i++) {
        free(split[i]);
    }
    free(split);
}

void printValid(int valid, char* str) {
    if (valid) {
        printf("%s successfully!\n", str);
    } else {
        printf("Fail to %s\n", str);
    }

    free(str);
}

void getInputAndProcess(FILE* fptr) {
    while (true) {
        char temp[1024];
        fgets(temp, 1024, stdin);
        if (strstr(temp, "###")) {
            return;
        }
        if (temp[0] == '\n') {
            continue;
        }

        if (strstr(temp, "print_order")) {
            int split_index = 0;
            char** split = delim(temp, &split_index);

            int table_id = strtol(split[2], NULL, 10);
            printf("%d\n", table_id);
            Table* table = searchTable(table_id);

            printOrder(table, false, fptr);

            // loại bỏ kí tự '#'
            fgets(NULL, 10, NULL);

            freeSplit(split, split_index);


        } else if (strstr(temp, "search_order")) {
            char buffer[1024];
            fgets(buffer, 1023, stdin);
            int table_id = strtol(buffer, NULL, 10);
            Table* table = searchTable(table_id);
            printf("Pointer to order list: %p\n\n", table->order);
        } else if (strstr(temp, "create_order")) {
            while (true) {
                char staff_id[20];
                int table_id;
                char buffer[1024];
                fgets(buffer, 1023, stdin);
                if (buffer[0] == '#') {
                    break;
                }

                sscanf(buffer, "%s %d", staff_id, &table_id);

                char* time = returnTime();
                createOrder(table_id, staff_id, time);

            }
        } else if (strstr(temp, "add_dish")) {
            while (true) {
                char buffer[1024];
                fgets(buffer, 1023, stdin);
                if (buffer[0] == '#') {
                    break;
                }

                char staff_id[20];
                int table_id;
                char dish[20];
                int quantity;
                char note[50];

                int split_size;
                char** split = delim(buffer, &split_size);
                strcpy(staff_id, split[0]);
                table_id = strtol(split[1], NULL, 10);
                strcpy(dish, split[2]);
                quantity = strtol(split[3], NULL, 10);
                strcpy(note, split[4]);


                char* time = returnTime();
                addDish(table_id, staff_id, time, dish, quantity, note);


                freeSplit(split, split_size);
            }

        } else if (strstr(temp, "update_dish")) {
            while (true) {
                char buffer[1024];
                fgets(buffer, 1023, stdin);
                if (buffer[0] == '#') {
                    break;
                }

                char staff_id[20];
                int table_id;
                char dish[20];
                int quantity;

                int split_size;
                char** split = delim(buffer, &split_size);
                strcpy(staff_id, split[0]);
                table_id = strtol(split[1], NULL, 10);
                strcpy(dish, split[2]);
                quantity = strtol(split[3], NULL, 10);

                int valid = updateDish(table_id, staff_id, dish, quantity);
                printValid(valid, strdup("Update Dish"));

                freeSplit(split, split_size);
            }
        } else if (strstr(temp, "cancel_dish")) {

            while (true) {
                char buffer[1024];
                fgets(buffer, 1023, stdin);
                if (buffer[0] == '#') {
                    break;
                }


                int table_id;
                char dish[20];
                char note[20];

                int split_size;
                char** split = delim(buffer, &split_size);

                table_id = strtol(split[0], NULL, 10);
                strcpy(dish, split[1]);
                strcpy(note, split[2]);

                int valid = cancelDish(table_id, dish, note);
                printValid(valid, strdup("Cancel Dish"));

                freeSplit(split, split_size);
            }
        } else if (strstr(temp, "cancel_order")) {
            while (true) {
                char buffer[1024];
                fgets(buffer, 1023, stdin);
                if (buffer[0] == '#') {
                    break;
                }

                int table_id = strtol(buffer, NULL, 10);

                int valid = cancelOrder(table_id);
                printValid(valid, strdup("Cancel Order"));
            }
        } else if (strstr(temp, "create_bill")) {
            while (true) {
                char buffer[1024];
                fgets(buffer, 1023, stdin);
                if (buffer[0] == '#') {
                    break;
                }

                char staff_id[20];
                int table_id;
                sscanf(buffer, "%s %d", staff_id, &table_id);
                createBill(table_id, fptr);
            }
        }
    }
}

void freeDishes(Dish* dishes) {
    Dish* cur = dishes;
    Dish* prev;
    while (cur) {
        prev = cur;
        cur = cur->next;
        free(prev);
    }
}

void freeOrders(Order* order) {
    Order* cur = order;
    Order* prev;
    while (cur) {
        prev = cur;
        cur = cur->next;
        freeDishes(prev->dishes);
        free(prev);
    }
}

void freeTable(Table* table) {
    Table* cur = table;
    Table* prev;

    while (cur) {
        prev = cur;
        cur = cur->next;
        freeOrders(prev->order);
        free(prev);
    }
}



int orderManagers() {
    char file_name[] = "orderManagers.txt";
    FILE* fptr = fopen(file_name, "a+");
    if (!fptr) {
        fprintf(stderr, "Cannot open the file!");
        exit(EXIT_FAILURE);
    }

    table_list = NULL;

    getInputAndProcess(fptr);


    freeTable(table_list);
    table_list = NULL;

    fclose(fptr);
    puts("Dữ liệu đã được ghi vào file để lưu trữ!");
    return 0;
}