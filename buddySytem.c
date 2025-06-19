//
// Created by HP LAPTOP on 14/06/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define MEMORY_SIZE 1024
#define MIN_SIZE 64

typedef struct MEMORY_MANAGER {
    int cur_size;
    bool is_used;
} MEMORY_MANAGER;


typedef struct MALLOC_MEMORY {
    int start_index;
    int request_size;
    int alloc_size;
} MALLOC_MEMORY;


// dùng cho buddy system
MEMORY_MANAGER* memory_manager;

// tách các phần theo cơ số của 2
// bản chất là cố gắng chia nhỏ để dùng best fit
// việc tính toán thân thiện vì được aligned theo memory address boundaries
// khi chia tạo ra 2 buddy
// khi merge thì chỉ 2 buddy cùng size nếu đều được free sẽ merge và cứ thế merge lên
// nên đặt giới hạn thấp nhất tránh việc quá tốn bộ nhớ cho metadata và thời gian theo dõi
// traverse binary tree ẩn với việc di chuyển giữa các lá dùng XOR

// kiểm tra nhanh là lũy thừa của 2 với size & (size - 1) == 0
void init_buddies_memory(int size) {
    if ((size & (size - 1)) != 0) {
        puts("Kích thước memory phải là lũy thừa của 2!");
    }

    memory_manager = (MEMORY_MANAGER*)malloc(size * sizeof(MEMORY_MANAGER));
    // ban đầu chỉ có đầu mảng quản lí
    memory_manager[0].cur_size = size;
    memory_manager[0].is_used = false;

    for (int i = 1; i < size; i++) {
        memory_manager[i].cur_size  = 0;
        memory_manager[i].is_used = false;
    }
}

void update_memory(int index, int new_size, bool is_used) {
    memory_manager[index].cur_size = new_size;
    memory_manager[index].is_used = is_used;
}

MALLOC_MEMORY* create_malloc_block(int start_index, int request_size, int alloc_size) {

    MALLOC_MEMORY* new_mem = (MALLOC_MEMORY*)malloc(sizeof(MALLOC_MEMORY));
    new_mem->start_index = start_index;
    new_mem->alloc_size = alloc_size;
    new_mem->request_size = request_size;
    return new_mem;
}

// hàm cấp phát, trả về khối được cấp phát
MALLOC_MEMORY* binary_fraction(int cur_index, int request_size) {
    // traverse hết cả cây rồi
    if (cur_index >= MEMORY_SIZE) {
        puts("Không đủ bộ nhớ để cấp phát!");
        return NULL;
    }


    // dùng con trỏ thay vì copy cả struct
    MEMORY_MANAGER* cur = &(memory_manager[cur_index]);
    int cur_size = cur->cur_size;
    int buddy_index = cur_index ^ cur_size;


    if (cur->is_used == false) {    // nếu chưa được dùng
        // lớn hơn gấp đôi khối cần cấp phát và cả min
        if (cur_size >= (request_size << 1) && cur_size > MIN_SIZE) {

            // tạo 2 buddies con
            int new_size = cur_size >> 1;
            int new_buddy_index = cur_index ^ new_size;

            // thêm 2 nút con vào cây
            update_memory(cur_index, new_size, false);
            update_memory(new_buddy_index, new_size, false);

            // xin bộ nhớ từ con trái
            return binary_fraction(cur_index, request_size);

        } else if (cur_size >= request_size) {
            // nếu vừa trong 1 block
            // sử dụng
            cur->is_used = true;

            // trả về block
            MALLOC_MEMORY* ret_mem = create_malloc_block(cur_index, request_size, cur_size);
            return ret_mem;
        }
    }

    // được dùng rồi hoặc là size không đủ

    int new_index = cur_index + cur_size;

    return binary_fraction(new_index, request_size);

}

void print_buddy_block(int start_index, int cur_block_size) {
    printf("Address: %p Size: %d\n", memory_manager + start_index, cur_block_size);
}

// chạy tuyến tính các nút lá từ trái qua phải với new_index = cur_index + cur_size
void print_available_buddies_block(int cur_index) {
    if (cur_index >= MEMORY_SIZE) {
        puts("");
        return;
    }

    MEMORY_MANAGER* cur = &(memory_manager[cur_index]);
    int cur_size = cur->cur_size;

    if (cur->is_used == false) {
        print_buddy_block(cur_index, cur_size);
    }

    int new_index = cur_index + cur_size;

    print_available_buddies_block(new_index);


}

// chạy đệ quy từ gốc đến lá
// và chia dần thành 2 nhánh
void print_available_buddies_block_ver2(int cur_index, int cur_mem_size) {
    MEMORY_MANAGER* cur = &(memory_manager[cur_index]);
    int cur_size = cur->cur_size;

    // nếu bằng nhau thì nó chính là lá
    if (cur_size == cur_mem_size) {
        if (cur->is_used == false) {
            print_buddy_block(cur_index, cur_size);
        }
        return;

    } else {    // không thì tức là nó có con, đi xuống sâu tiếp
        // chia đôi size ước tính
        int child_size = cur_mem_size >> 1;
        int buddy_index = cur_index ^ child_size;

        // đệ quy xuống con trái trước
        print_available_buddies_block_ver2(cur_index, child_size);
        print_available_buddies_block_ver2(buddy_index, child_size);
    }
}

// kiểm tra là buddy với buddy_index = cur_index ^ cur_size
void merge_buddies_mem(int free_index) {
    int free_size = memory_manager[free_index].cur_size;
    if (free_size >= MEMORY_SIZE) { return; }

    memory_manager[free_index].is_used = false;

    int buddy_index = free_index ^ free_size;
    int left_index = (free_index < buddy_index)? free_index: buddy_index;

    if (memory_manager[buddy_index].is_used == false) {
        int merge_size = free_size << 1;

        int right_index = left_index ^ free_size;

        update_memory(right_index, 0, false);

        update_memory(left_index, merge_size, false);

        merge_buddies_mem(left_index);
    }

}


void free_buddies_mem(MALLOC_MEMORY* memory_ptr) {

    int free_index = memory_ptr->start_index;

    merge_buddies_mem(free_index);

    free(memory_ptr);
}

int buddySystem() {
    

    init_buddies_memory(MEMORY_SIZE);

    MALLOC_MEMORY* ptr1 = binary_fraction(0, 20);

    print_available_buddies_block_ver2(0, MEMORY_SIZE);
    puts("");

    MALLOC_MEMORY* ptr2 = binary_fraction(0, 70);

    print_available_buddies_block_ver2(0, MEMORY_SIZE);
    puts("");

    free_buddies_mem(ptr2);

    print_available_buddies_block(0);

    MALLOC_MEMORY* ptr3 = binary_fraction(0, 129);

    print_available_buddies_block_ver2(0, MEMORY_SIZE);
    puts("");

    free_buddies_mem(ptr1);

    print_available_buddies_block_ver2(0, MEMORY_SIZE);
    puts("");

    free_buddies_mem(ptr3);

    print_available_buddies_block_ver2(0, MEMORY_SIZE);
    puts("");

    free(memory_manager);
}