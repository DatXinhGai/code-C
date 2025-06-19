//
// Created by HP LAPTOP on 11/06/2025.
//
#include <bemapiset.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

// bên linux thì thoải mái, bên win chỉ cố định 260 thôi
#define MAX_NAME 1024


char root_dir_name[MAX_NAME];

const char directory_name[MAX_NAME] = "C://Haha";


struct stat st;

int getCurrentWorkingDir() {
    // hàm lấy PATH đến thư mục hiện tại
    if (getcwd(root_dir_name, sizeof(root_dir_name)) != NULL) {
        printf("Current working directory: %s\n", root_dir_name);
        return 0;
    } else {
        perror("Lỗi khi lấy cwd");
        return 1;
    }
}

void list_files_and_folders(const char* root_dir, int indent) {
    // con trỏ làm việc với directory
    DIR* dir = NULL;

    struct dirent* entry;
    // mở dir
    if ((dir = opendir(root_dir)) == NULL) {
        perror("opendir() error");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        for (int i = 0; i < indent; i++) {
            printf(" ");
        }

        printf(entry->d_name);
        // không hỗ trọ
        /*
        if (entry->d_type == DT_DIR) {

        }
        */
        char path[MAX_NAME];
        snprintf(path, sizeof(path), "%s/%s", root_dir, entry->d_name);
        puts("\n");

        if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
            list_files_and_folders(path, indent + 1);
        }
    }

}

void createNewDir() {
    int status = mkdir(directory_name);
    if (status == 0) {
        printf("Tạo thư mục mới thành công\n");
    } else {
        perror("Lỗi createDir");
        exit(EXIT_FAILURE);
    }

}

int linuxDir() {
    getCurrentWorkingDir();
    list_files_and_folders(root_dir_name, 3);
    createNewDir();
    return 0;
}

