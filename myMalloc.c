//
// Created by HP LAPTOP on 03/04/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>


// tối ưu thì phải binary search tree(BST) để kiểm soát
// cấp phát -> frac (gồm vừa đủ hoặc thừa) -> initialize
// thu hồi -> merge (insert hoặc push)
// free_mem


typedef struct MEMORY_BLOCK {
    void* start_addr;           // địa chỉ bắt đầu
    size_t size;                // size của block
    struct MEMORY_BLOCK* next;  // block tiếp theo
} MEMORY_BLOCK;


// ở ngoài hàm nên là static, khỏi truyền vào ** và không phải khai báo ở hàm
MEMORY_BLOCK* free_list = NULL;



// lưu vị trí tiếp theo của khối vừa cấp phát gần nhất để dùng cho next_fit
MEMORY_BLOCK* cur_last_pos = NULL;
MEMORY_BLOCK* prev_last_pos = NULL;

void print_free_list();

void print_stderr() {
    fprintf(stderr, "Allocated Unsuccessfully\n");
    exit(EXIT_FAILURE);

}

// hàm khởi tạo bộ nhớ
void initialize_memory(void* base_adder, size_t size) {

    free_list = (MEMORY_BLOCK*)malloc(sizeof(MEMORY_BLOCK));

    free_list->start_addr = base_adder;
    free_list->size = size;
    free_list->next = NULL;
}

// hàm tạo khối khi merge hoặc frac
MEMORY_BLOCK* initialize_block(void* ptr, size_t size) {
    MEMORY_BLOCK* new_block = (MEMORY_BLOCK*)malloc(sizeof(MEMORY_BLOCK));
    new_block->start_addr = ptr;
    new_block->size = size;
    new_block->next = NULL;
    return new_block;
}

// hàm fragment khối free_list khi malloc
void frac(MEMORY_BLOCK* current, MEMORY_BLOCK* prev, size_t size) {

    // cấp phát thừa
    if (current->size > size) {
        // quản lí phần dư còn lại
        void* remaining_adder = current->start_addr + size;
        size_t remaining_size = current->size - size;
        MEMORY_BLOCK* remaining = initialize_block(remaining_adder, remaining_size);
        remaining->next = current->next;

        if (prev == NULL) {
            free_list = remaining;
        } else {
            prev->next = remaining;
        }
    } else {    // vừa đủ

        if (prev == NULL) {
            free_list = current->next;
        } else {
            prev->next = current->next;
        }
    }

    // LƯU LẠI ĐỂ DÙNG CHO NEXT_FIT
    cur_last_pos = (prev == NULL)? free_list : prev->next;
    prev_last_pos = (prev == NULL)? NULL : prev;
}

// hàm merge khi khối được trả lại nằm sau free_list
void insert_memory_block(void* ptr, size_t size) {

    MEMORY_BLOCK* base_block = free_list;
    // tìm vị trí cuối cùng đứng trước ptr
    while (base_block->next != NULL && base_block->next->start_addr < ptr) {
        base_block = base_block->next;
    }

    MEMORY_BLOCK* remaining_block = base_block->next;

    // xử lí phần sau
    if (remaining_block != NULL) {  // nếu đằng sau tồn tại khối
        // phần sau khớp
        if (ptr + size == remaining_block->start_addr) {

            remaining_block->size += size;        // tăng size
            remaining_block->start_addr = ptr;    // chỉnh phần bắt đầu của remaining
        } else {    // không khớp
            MEMORY_BLOCK* new_block = initialize_block(ptr, size);
            new_block->next = remaining_block;
            remaining_block = new_block;
        }
    }

    // xử lí phần đầu
    if (remaining_block != NULL &&
        base_block->start_addr + base_block->size == remaining_block->start_addr) {
        // vừa đủ
        base_block->size += remaining_block->size;
        base_block->next = remaining_block->next;
        free(remaining_block);   // không cần remaining để quản lí nữa
    } else {
        base_block->next = remaining_block;
    }

}

// hàm merge khi phần trả về nằm trước free_list
void push_memory_block(void* ptr, size_t size) {

    MEMORY_BLOCK* new_block = initialize_block(ptr, size);
    // nếu mà vừa khớp để merge
    if (ptr + size == free_list->start_addr) {
        MEMORY_BLOCK* temp = free_list;
        new_block->size += free_list->size;
        new_block->next = free_list->next;
        free(temp);
    } else {
        new_block->next = free_list;
    }

    free_list = new_block;
}

// hàm hợp nhất tất cả cái khối có thể
// thực ra chỉ cần xét đằng trước và sau của khối trả về thôi
// tìm được thì có thể kết thúc sớm
void merge_memory_block() {
    MEMORY_BLOCK* cur_block = free_list;

    while (cur_block->next != NULL) {
        MEMORY_BLOCK* next_block = cur_block->next;
        if (cur_block->start_addr + cur_block->size == next_block->start_addr) {
            cur_block->size += next_block->size;
            cur_block->next = next_block->next;
            free(next_block);   // merge cur_block và next_block nên không cần nữa
        } else {
            cur_block = next_block;
        }
    }
}

// trả lại bộ nhớ cho hệ thống
void free_mem(void* ptr, size_t size) {
    // trả lại
    if (ptr < free_list->start_addr) {
        push_memory_block(ptr, size);
    } else {
        insert_memory_block(ptr, size);
    }
    // merge tất cả các khối liền kề
    merge_memory_block();
}


//  tìm được thằng nào vừa thì return luôn
void* first_fit_malloc(size_t size) {
    MEMORY_BLOCK* prev = NULL;
    MEMORY_BLOCK* current = free_list;

    while (current != NULL) {
        if (current->size >= size) {
            void* addr = current->start_addr;
            // hàm phân mảnh bộ nhớ
            frac(current, prev, size);

            free(current);
            return addr;
        }
        prev = current;
        current = current->next;
    }

    print_stderr();
    return NULL;
}


// tìm khối nhỏ nhất mà lớn hơn bằng size
void* best_fit_malloc(size_t size) {
    MEMORY_BLOCK* prev = NULL;
    MEMORY_BLOCK* current = free_list;

    // vị trí sẽ cấp phát thực sự
    MEMORY_BLOCK* minPrev = NULL;
    MEMORY_BLOCK* minCurrent = NULL;

    size_t minSize = SIZE_MAX;
    bool SUCCESS = false;

    while (current != NULL) {
        if (current->size >= size) {
            SUCCESS = true;

            if (current->size < minSize) {
                minSize = current->size;
                minPrev = prev;
                minCurrent = current;
            }
        }
        prev = current;
        current = current->next;
    }

    if (SUCCESS) {
        void* addr = minCurrent->start_addr;

        frac(minCurrent, minPrev, size);

        free(minCurrent);
        return addr;
    }

    print_stderr();
    return NULL; // không cấp phát thành công
}


// tìm khối lớn nhất lớn hơn bằng size
void* worst_fit_malloc(size_t size) {
    MEMORY_BLOCK* prev = NULL;
    MEMORY_BLOCK* current = free_list;

    MEMORY_BLOCK* maxPrev = NULL;
    MEMORY_BLOCK* maxCurrent = NULL;

    size_t maxSize = 0;

    bool SUCCESS = false;

    while (current != NULL) {
        if (current->size >= size) {
            SUCCESS = true;

            if (current->size > maxSize) {
                maxSize = current->size;
                maxPrev = prev;
                maxCurrent = current;
            }
        }

        prev = current;
        current = current->next;
    }

    if (SUCCESS) {
        void* addr = maxCurrent->start_addr;

        frac(maxCurrent, maxPrev, size);

        free(maxCurrent);

        return addr;
    }

    print_stderr();
    return NULL;

}


// tìm từ phần cấp phát trước đó
void* next_fit_malloc(size_t size) {
    while (cur_last_pos != NULL) {
        if (cur_last_pos->size >= size) {
            MEMORY_BLOCK* temp = cur_last_pos;
            void* addr = cur_last_pos->start_addr;

            // sau bước này cur_last_pos thay đổi vì vậy phải lưu biến khác để free!
            frac(cur_last_pos, prev_last_pos, size);

            free(temp);
            return addr;
        }

        prev_last_pos = cur_last_pos;
        cur_last_pos = cur_last_pos->next;
    }

    print_stderr();
    return NULL;
}



void* my_realloc(void* ptr, size_t cur_size, size_t new_size) {
    // ktr khi ptr == NULL vì malloc(size) == realloc(NULL, size) khi đó chỉ cần cấp phát
    if (ptr != NULL) {
        free_mem(ptr, cur_size);
    }
    return first_fit_malloc(new_size);
}



void* my_calloc(size_t mem_count, size_t mem_size) {
    void* addr = first_fit_malloc(mem_count*mem_size);

    if (addr) {
        memset(addr, 0, mem_count*mem_size);
    }

    return addr;

}



void print_free_list() {
    MEMORY_BLOCK* cur = free_list;
    if (cur == NULL) {
        printf("There is no valid memory block\n");
        return ;
    }

    printf("Free Memory Block:\n");
    while (cur != NULL) {
        printf("Address: %p \t Size: %zu\n", cur->start_addr, cur->size);
        cur = cur->next;
    }
}

void destroy_memory() {
    MEMORY_BLOCK* cur = free_list;

    while (cur != NULL) {
        MEMORY_BLOCK* temp = cur;
        cur = cur->next;
        free(temp);
    }

    free_list = NULL;
}



// hàm đệ quy chia khối của buddy system
void binary_frac(MEMORY_BLOCK* current, size_t size) {
    if (current->size <= 2*size) {
        return ;
    }
    // lấy 1 nửa
    size_t padding = current->size / 2;
    // tạo block mới ở bên phải
    MEMORY_BLOCK* mid_block = initialize_block(current->start_addr + padding, padding);

    // gắn block mới vào và cập nhật lại size của current
    mid_block->next = current->next;
    current->next = mid_block;
    current->size = padding;
    // gọi đệ quy chia tiếp đến khi không được nữa
    binary_frac(current, size);
}

void* buddy_system_malloc(size_t size) {
    MEMORY_BLOCK* prev = NULL;
    MEMORY_BLOCK* current = free_list;

    while (current != NULL) {
        if (current->size >= size) {

            void* addr = current->start_addr;

            binary_frac(current, size);

            frac(current, prev, size);

            free(current);
            return addr;
        }

        prev = current;
        current = current->next;
    }

    print_stderr();
    return NULL;
}




int myMalloc() {

    void* base_adder = (void*)malloc(4096);
    initialize_memory(base_adder, 4096);

    print_free_list();
    void* ptr1 = first_fit_malloc(128);
    print_free_list();

    void* ptr2 = best_fit_malloc(256);
    print_free_list();

    void* ptr3 = best_fit_malloc(512);
    print_free_list();

    void* ptr4 = best_fit_malloc(128);
    print_free_list();

    free_mem(ptr3, 512);
    print_free_list();
    free_mem(ptr1, 128);
    print_free_list();


    void* ptr6 = worst_fit_malloc(256);
    print_free_list();

    void* ptr7 = my_realloc(ptr6, 256, 64);
    print_free_list();

    void* ptr8 = my_calloc(256, sizeof(void));
    print_free_list();

    // tiếp vừa ở chỗ ptr8
    void* ptr9 = next_fit_malloc(64);
    print_free_list();

    void* ptr10 = buddy_system_malloc(256);
    print_free_list();

    void* ptr11 = buddy_system_malloc(3);
    print_free_list();

    free_mem(ptr4, 128);
    print_free_list();

    free_mem(ptr2, 256);
    print_free_list();

    free_mem(ptr7, 64);
    free_mem(ptr8, 256);
    print_free_list();

    free_mem(ptr9, 64);
    print_free_list();

    free_mem(ptr10, 256);
    print_free_list();

    free_mem(ptr11, 3);
    print_free_list();

    destroy_memory();
    free(base_adder);

    return 0;
}




