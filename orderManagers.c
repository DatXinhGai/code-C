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

typedef struct DishList {
    Dish* first;
    Dish* last;
} DishList;

struct Order {
    char created_time[MAX_TIME];
    char staff_id[MAX_NAME];
    DishList* dishes_list;
    int dishes_count;
    int nums_count;
    int returned_dishes;
    int returned_nums;
    char updated_time[MAX_TIME];
    Status status;
    struct Order* next;
};

typedef struct Order Order;

typedef struct OrderList {
    Order* first;
    Order* last;
} OrderList;

struct Table {
    int table_id;
    OrderList* order_list;
    struct Table* next;
};

typedef struct Table Table;

typedef struct TableList {
    Table* first;
    Table* last;
} TableList;

char* returnTime() {
    // struct quản lí thời gian
    struct tm* ptr;
    time_t t;

    t = time(NULL);

    // lấy thời gian khu vực
    // muốn lấy theo múi 0h thì gmtime
    ptr = localtime(&t);

    // ghi thời gian theo định dạng
    char* str = asctime(ptr);
    //str[strcspn(str, "\r\n")] = '\0';

    return str;

}

void printToStream(char* message, FILE* fptr) {
    fputs(message, fptr);
}

void printAllStream(char* message, FILE* fptr) {
    printToStream(message, stdout);
    printToStream(message, fptr);
}

TableList* table_list = NULL;

// tìm bàn trong danh sách bàn qua id

// tạo danh sách bàn
void createTableList() {
    table_list = (TableList*)malloc(sizeof(TableList));
    table_list->first = table_list->last = NULL;
}

Table* searchTable(int table_id) {
    if (!table_list) {
        return NULL;
    }

    Table* cur = table_list->first;

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
    table->order_list = NULL;
    table->table_id = table_id;
    table->next = NULL;

    return table;
}

// thêm bàn mới vào danh sách bàn hiện có, nếu chưa có danh sách bàn thì tạo mới
void addTableToLast(Table* table) {
    if (table_list == NULL) { createTableList(); }

    if (table_list->last == NULL) {
        table_list->first = table_list->last = table;
    } else {
        table_list->last->next = table;
        table_list->last = table;
    }
}

// tìm và trả về đơn mới nhất của bàn
Order* searchOrder(int table_id) {
    Table* cur = table_list->first;
    while (cur != NULL) {
        if (cur->table_id == table_id) {
            return cur->order_list->first;
        }

        cur = cur->next;
    }

    puts("Bàn chưa gọi món!");
    return NULL;
}

// tạo danh sách đơn mới
OrderList* createOrderList() {
    OrderList* order_list = (OrderList*)malloc(sizeof(OrderList));
    order_list->first = order_list->last = NULL;
    return order_list;
}

// tạo order và thêm vào cuối của table
// nếu chưa có thì tạo mới bàn
// chưa có danh sách order cũng tạo mới
Order* createOrder(int table_id, char staff_id[], char time[]) {

    Table* table = searchTable(table_id);
    // tạo bàn
    if (!table) {
        table = createTable(table_id);
        addTableToLast(table);
    }
    // đã có sẵn order đang được phục vụ rồi
    if (table->order_list && table->order_list->first && table->order_list->first->status == SERVING) {
        puts("Bàn đã được tạo order!");
        return NULL;
    }

    Order* new_order = (Order*)malloc(sizeof(Order));

    strcpy(new_order->created_time, time);
    new_order->dishes_list = NULL;
    new_order->dishes_count = 0;
    new_order->nums_count = 0;
    new_order->returned_dishes = 0;
    new_order->returned_nums = 0;
    new_order->status = SERVING;
    strcpy(new_order->staff_id, staff_id);

    // cập nhật danh sách order cho bàn
    if (!table->order_list) {
        table->order_list = createOrderList();
    }

    new_order->next = table->order_list->first;
    table->order_list->first = new_order;

    if (table->order_list->last == NULL) {
        table->order_list->last = new_order;
    }

    return new_order;
}

// khi order bị hủy hoặc được thanh toán thì chuyển về cuối danh sách
void updateOrder(Table* table) {

    char* time = returnTime();

    Order* first = table->order_list->first;

    strcpy(first->updated_time , time);

    if (first->next == NULL) {
        return ;
    }

    table->order_list->last->next = first;
    table->order_list->first = first->next;
    table->order_list->last = first;
    first->next = NULL;
}

// hủy đơn khi chưa trả món nào
int cancelOrder(int table_id) {
    Table* table = searchTable(table_id);

    Order* first_order = table->order_list->first;

    if (!table || !first_order || first_order->status != SERVING) {
        return 0;
    }
    // đã trả món thì không hủy được nữa
    if (first_order->returned_nums != 0) {
        return 0;
    }

    first_order->dishes_count = 0;
    first_order->nums_count = 0;
    // gán order bị hủy và update đưa về cuối và không cần in ra nữa
    first_order->status = CANCEL;
    updateOrder(table);
    return 1;
}

void createDishList(Order* order) {
    order->dishes_list = (DishList*)malloc(sizeof(DishList));
    order->dishes_list->first = NULL;
    order->dishes_list->last = NULL;
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

// hàm thêm món
// có thể tạo mới ds bàn, bàn, ds order, order, dish_list;
int addDish(int table_id, char staff_id[], char time[], char dish_id[], int count, char note[]) {

    if (!table_list) { createTableList(); }

    Table* table = searchTable(table_id);

    if (!table) {
        table = createTable(table_id);
        addTableToLast(table);
    }

    if (!table->order_list) {
        table->order_list = createOrderList();
    }

    // chưa có order hoặc các order trước đó đã xong hoặc hủy

    Order* first_order = table->order_list->first;
    if (!first_order || first_order->status == PAID || first_order->status == CANCEL) {
        // tạo order mới cho bàn và gắn vào đầu
        Order* new_order = createOrder(table_id, staff_id, time);

        if (!first_order) {
            table->order_list->first = table->order_list->last = new_order;
        } else {
            new_order->next = table->order_list->first;
            table->order_list->first = new_order;
        }
        return 1;
    }

    if (!first_order->dishes_list) {
        createDishList(first_order);
    }

    // nếu có đơn đang phục vụ thì tìm món
    Dish* cur_dish = first_order->dishes_list->first;
    bool dish_find = false;

    while (cur_dish != NULL) {
        if (strcmp(cur_dish->id, dish_id) == 0) {
            dish_find = true;
            break;
        }
        cur_dish = cur_dish->next;
    }

    // không tìm được thì tạo món mới
    if (dish_find == false) {
        Dish* new_dish = createDish(dish_id, count, note);
        if (!first_order->dishes_list->first) {
            first_order->dishes_list->first = first_order->dishes_list->last = new_dish;
        } else {
            first_order->dishes_list->last->next = new_dish;
            first_order->dishes_list->last = new_dish;
        }
        // tăng món cho order
        first_order->dishes_count++;

    } else {    // tìm được
        // món đã hoàn thành nhưng gọi thêm thì coi như là chưa hoàn thành, và khi đó dish_count không đổi
        if (cur_dish->ordered_nums == cur_dish->returned_nums) {
            first_order->returned_dishes--;
        }

        // cập nhật dish
        cur_dish->ordered_nums += count;
        strcpy(cur_dish->note, note);

        table->order_list->first->nums_count += count;
    }

    first_order->nums_count += count;

    return 1;
}


int updateDish(int table_id, char staff_id[], char dish_id[], int count) {
    Table* table = searchTable(table_id);
    if (!table) {
        return 0;
    }

    Order* order = table->order_list->first;
    if (!order || order->status != SERVING) {
        return 0;
    }

    Dish* return_dish = searchDish(order->dishes_list->first, dish_id);

    if (!return_dish) {
        puts("Khách không order món này, không trả\n");
    }
    else {
        // tính số đĩa còn cần phải phục vụ
        int serving_nums = return_dish->ordered_nums - return_dish->returned_nums;
        if (serving_nums < count) {
            puts("Trả món quá số lượng\n");
            count = serving_nums; // chỉnh lại về số món tối đa được trả
        }

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

    return 0;
}


int cancelDish(char staff_id[], int table_id, char dish_id[], char note[]) {
    Table* table = searchTable(table_id);

    if (!table || !table->order_list || !table->order_list->first->dishes_list) {
        return 0;
    }

    Dish* return_dish = searchDish(table->order_list->first->dishes_list->first, dish_id);

    strcpy(table->order_list->first->staff_id, staff_id);

    if (return_dish) {
        // nếu chưa đĩa nào trả
        if (return_dish->returned_nums == 0) {
            int count = return_dish->ordered_nums;

            return_dish->ordered_nums = 0;
            strcpy(return_dish->note, note);

            table->order_list->first->dishes_count--;
            table->order_list->first->nums_count -= count;

            return 1;
        }
    }
    return 0;
}



void printDish(Dish* dish, bool paid, FILE* fptr) {
    if (dish == NULL) {
        return ;
    }

    char message[1024] ;

    sprintf(message, "\tMón: %-20s    Số đĩa đặt: %-6d     Số đĩa đã trả: %-6d     Ghi chú: %s\n",
            dish->id, dish->ordered_nums, dish->returned_nums, dish->note
        );

    printAllStream(message, fptr);

    printDish(dish->next, paid, fptr);
}



void printOrder(Table* table, bool paid,  FILE* fptr) {
    Order *order = table->order_list->first;

    if (order == NULL) {
        return ;
    }
    // cập nhật thời gian update
    char* time = returnTime();
    strcpy(order->updated_time, time);


    char message[1024];

    // sprintf không như fwrite hay fprintf có thể tiếp tục vào luồng
    // mà nó sẽ xóa chuỗi và ghi mới
    // có thể dùng snprintf với offset hoặc ghi vào buffer sau đó strcat
    // cuối cùng fwrite để ghi theo khối, tuy nhiên sẽ không hiểu được 

    sprintf(message, "Bàn được phục vụ là bàn số: %d\n", table->table_id);
    printAllStream(message, fptr);

    sprintf(message, "Order được tạo lúc: %s\n", order->created_time);
    printAllStream(message, fptr);

    char staff_info[1024];
    sprintf(staff_info, "Nhân viên phục vụ: %s\n", order->staff_id);
    printAllStream(staff_info, fptr);

    printDish(order->dishes_list->first, paid, fptr);

    sprintf(message, "Tổng số món đã đặt là: %d\n", order->dishes_count);
    printAllStream(message, fptr);

    sprintf(message, "Tổng số đĩa đã đặt là: %d\n", order->nums_count);
    printAllStream(message, fptr);

    sprintf(message, "Tổng số món đã trả là: %d\n", order->returned_dishes);
    printAllStream(message, fptr);

    sprintf(message, "Tống số đĩa đã trả là: %d\n", order->returned_nums);
    printAllStream(message, fptr);

    sprintf(message, "Thời gian cập nhật gần nhất của order là: %s", order->updated_time);
    printAllStream(message, fptr);

    if (paid) {
        order->status = PAID;
        updateOrder(table);

        sprintf(message, "Thanh toán thành công!\n");
        printAllStream(message, fptr);
    }
}


void printTable(Table* table, FILE* fptr) {
    if (table == NULL) {
        return  ;
    }

    char s[100] = "------------------------------------------";
    puts(s);
    fprintf(fptr, "%s\n", s);

    printOrder(table, true, fptr);

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

// hàm tách các thành phần của đầu vào
// chia ra các trường hợp đã mở "" hay chưa
char** delim(char* str, int* split_index) {
    str[strcspn(str, "\r\n")] = '\0';
    *split_index = 0;
    char** split = (char**)malloc(MAX_SIZE * sizeof(char*));

    char* n_ptr = str;
    char* end_ptr = str;
    bool open = false;

    while (*n_ptr != '\0') {
        if (open) {
            if (*n_ptr == '"') {
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
            } else if (*n_ptr == '"') {
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

            Table* table = searchTable(table_id);

            printOrder(table, false, fptr);

            // loại bỏ kí tự '#'
            char garbage[16];
            fgets(garbage, 10, stdin);

            freeSplit(split, split_index);


        } else if (strstr(temp, "search_order")) {

            char buffer[1024];
            fgets(buffer, 1023, stdin);
            int table_id = strtol(buffer, NULL, 10);
            Table* table = searchTable(table_id);
            printf("Pointer to order list: %p\n\n", table->order_list->first);

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

                char staff_id[MAX_NAME];
                int table_id;
                char dish[MAX_NAME];
                char note[MAX_NOTE];

                int split_size;
                char** split = delim(buffer, &split_size);

                strcpy(staff_id, split[0]);

                table_id = strtol(split[1], NULL, 10);
                strcpy(dish, split[2]);
                strcpy(note, split[3]);

                int valid = cancelDish(staff_id, table_id, dish, note);
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
        freeDishes(prev->dishes_list->first);
        free(prev->dishes_list);
        free(prev);
    }
}

void freeTable(Table* table) {
    Table* cur = table;
    Table* prev;

    while (cur) {
        prev = cur;
        cur = cur->next;
        freeOrders(prev->order_list->first);
        free(prev->order_list);
        free(prev);
    }
}



int orderManagers() {
    char file_name[] = "orderManagers.txt";
    FILE* fptr = fopen(file_name, "a+b");
    if (!fptr) {
        fprintf(stderr, "Cannot open the file!");
        exit(EXIT_FAILURE);
    }

    table_list = NULL;

    getInputAndProcess(fptr);


    freeTable(table_list->first);
    free(table_list);
    table_list = NULL;

    fclose(fptr);
    puts("Dữ liệu đã được ghi vào file để lưu trữ!");
    return 0;
}
