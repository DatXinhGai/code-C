//
// Created by HP LAPTOP on 11/06/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <wchar.h>

#define MAX_NAME 260

typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define INDENT 3

enum Status {
    ASC = 1,
    DEC = 2,
};

typedef enum Status Status;

WCHAR root_directory[MAX_NAME];
DWORD dwRet;

typedef struct TreeNode {
    WCHAR name[MAX_NAME];
    WCHAR path[MAX_NAME];
    BOOL isDirectory;

    struct TreeNode* parent;
    struct TreeNode* firstChild;
    struct TreeNode* nextSibling;
} TreeNode;

// CẤU TRÚC DỮ LIỆU VÀ THUẬT TOÁN HỖ TRỢ
// quick sort
void swapDir(WCHAR** a, WCHAR** b) {
    LPWSTR temp = *a;
    *a = *b;
    *b = temp;
}


// lưu ý không dùng DWORD, unsigned long nên left - 1 có thể xoay vòng
// phương pháp chọn pivot theo random
int LomutoPartition(WCHAR** dir_arr, int left, int right) {

    int size = right - left + 1;
    int rand_num = rand() % size;

    int pivotIndex = left + rand_num;
    swapDir(&dir_arr[pivotIndex], &dir_arr[right]);

    int i = left, j = left;

    for (i = left; i < right; i++) {
        if (wcscmp(dir_arr[i], dir_arr[right]) <= 0) {
            swapDir(&dir_arr[j++], &dir_arr[i]);
        }
    }

    swapDir(&dir_arr[j], &dir_arr[right]);

    return j;
}

void quickSort(WCHAR** dir_arr, int left, int right) {

    if (left >= right) {
        return ;
    }

    int pivotIndex = LomutoPartition(dir_arr, left, right);

    quickSort(dir_arr, left, pivotIndex - 1);
    quickSort(dir_arr, pivotIndex + 1, right);

}
//


// làm quen với WinAPI
// chỉ rõ W để phân biệt với A(ASCII)
int getCurDir() {
    dwRet = GetCurrentDirectoryW(MAX_NAME, root_directory);

    if (dwRet == 0 || dwRet > MAX_NAME) {
        printf("GetCurrentDirectory fail (%lu)", GetLastError());
        return 1;
    }

    wprintf(L"CurrentDirectory: %ls\n", root_directory);
    return 0;
}

LPWSTR getCurPath(LPCWSTR root_dir_name) {
    LPWSTR cur_path = (LPWSTR)malloc(MAX_NAME * sizeof(WCHAR));

    swprintf(cur_path, MAX_NAME, L"%ls\\*", root_dir_name);

    return cur_path;
}

void listDir(const WCHAR* root_dir, int indent) {
    WIN32_FIND_DATAW find_file_data;    // struct lưu thông tin về file lấy được
    HANDLE h_find = INVALID_HANDLE_VALUE;   // handle là con trỏ đặc biệt của win để xử lí
    WCHAR path[1024];


    // thêm dấu * để duyệt mọi file và thư mục
    // đường dẫn tới thư mục hiện tại
    swprintf(path, sizeof(path) / sizeof(WCHAR) , L"%ls\\*", root_dir);
    //wprintf(L"%ls\n", path);

    h_find = FindFirstFileW(path, &find_file_data); // tìm thư mục đầu tiên

    if (h_find == INVALID_HANDLE_VALUE) {
        wprintf(L"\nFindFirstFile fail (%lu)\n", GetLastError());
        return ;
    }

    int check;

    do {
        LPCWSTR name = find_file_data.cFileName;

        if (wcscmp(name, L".") == 0 || wcscmp(name, L"..") == 0) {
            continue;   // bỏ qua thư mục hiện tại và thư mục cha
        }

        check = 0;
        // check xem có là thư mục không, nếu có thì chuyển thành +
        if (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            printf("+");
            check = 1;
        }

        // chắc chắn thuộc các thư mục cha
        for (int i = check; i <= indent; i++) {
            printf("-");
        }

        wprintf(L"%ls\n", name);

        if (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            WCHAR sub_dir[MAX_NAME];
            swprintf(sub_dir, sizeof(sub_dir) / sizeof(WCHAR), L"%ls\\%ls", root_dir, name);
            listDir(sub_dir, indent + 1);    // Gọi đệ quy cho thư mục con
        }
    } while (FindNextFileW(h_find, &find_file_data) != 0);
    // đóng handle
    FindClose(h_find);
}

BOOL createDir(LPCWSTR root_dir_name) {

    // Tạo thư mục mới
    if (CreateDirectoryW(root_dir_name, NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
        wprintf(L"Directory '%ls' is created succesfully or already exist.\n", root_dir_name);
        return TRUE;
    } else {
        wprintf(L"Fail to create directory '%ls'. Error %lu\n", root_dir_name, GetLastError());
        return FALSE;
    }
}

void listCurrentFileAndFolders(const WCHAR* root_dir_name, DWORD sorted) {


    WIN32_FIND_DATAW find_data;
    HANDLE h_find = INVALID_HANDLE_VALUE;

    WCHAR path[MAX_NAME];
    swprintf(path, sizeof(path) / sizeof(WCHAR), L"%ls\\*", root_dir_name);

    h_find = FindFirstFileW(path, &find_data);

    if (h_find == INVALID_HANDLE_VALUE) {
        wprintf(L"FindFirstFile error (%lu)\n", GetLastError());
        return ;
    }

    do {
        LPCWSTR file_name = find_data.cFileName;

        wprintf(L"%ls\n", file_name);
    } while (FindNextFileW(h_find, &find_data));
}

//


// nếu được có thể thêm trường lastChild để giúp cho việc thêm con mới nhanh hơn
TreeNode* createDirNode(const WCHAR* name, LPCWSTR path, BOOL isDirectory) {
    TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));

    wcscpy(new_node->name, name);
    wcscpy(new_node->path, path);
    new_node->isDirectory = isDirectory;

    new_node->parent = NULL;
    new_node->firstChild = NULL;
    new_node->nextSibling = NULL;

    return new_node;
}

TreeNode* addDirChild(TreeNode* parent, const WCHAR* name, BOOL is_directory) {
    TreeNode* new_node = createDirNode(name, L"\0", is_directory);

    new_node->parent = parent;


    if (parent->firstChild == NULL) {
        parent->firstChild = new_node;
    } else {
        TreeNode* cur_child = parent->firstChild;
        while (cur_child->nextSibling != NULL) {
            cur_child = cur_child->nextSibling;
        }

        cur_child->nextSibling = new_node;
    }


    return new_node;
}

void printTree(TreeNode* root, DWORD indent) {
    if (root == NULL) { return; }
    for (DWORD i = 0; i < indent; i++) {
        wprintf(L"  ");
    }

    wprintf(L"|-%ls\n", root->name);

    TreeNode* child = root->firstChild;

    while (child) {
        printTree(child, indent + 1);
        child = child->nextSibling;
    }
}

// tư tưởng là post-order
// xử lí hết các anh chị em và các con
// rồi mới xử lí nó
// vì khi đó việc xóa cur_node không ảnh hưởng đến các node khác còn lại trên cây
void freeDirTree(TreeNode** root) {
    if (*root == NULL) { return; }

    if ((*root)->nextSibling) {
        freeDirTree(&((*root)->nextSibling));
    }
    if ((*root)->firstChild) {
        freeDirTree(&((*root)->firstChild));
    }

    free(*root);
    *root = NULL;
}

void removeDir(TreeNode* del) {
    TreeNode* cur_parent = del->parent;

    if (cur_parent->firstChild == del) {
        cur_parent->firstChild = del->nextSibling;

    } else {
        TreeNode* cur_child = cur_parent->firstChild;

        while (cur_child->nextSibling != del) {
            cur_child = cur_child->nextSibling;
        }

        cur_child->nextSibling = del->nextSibling;
    }

    del->parent = NULL;
    del->nextSibling = NULL;
}

void addDir(TreeNode* dest, TreeNode* source) {
    source->parent = dest;

    if (dest->firstChild == NULL) { dest->firstChild = source; }
    else {
        TreeNode* child = dest->firstChild;

        while (child->nextSibling != NULL) {
            child = child->nextSibling;
        }

        child->nextSibling = source;
    }

}

TreeNode* copyTree(TreeNode* source) {
    if (source == NULL) {
        return NULL;
    }

    TreeNode* dest = createDirNode(source->name, source->path, source->isDirectory);


    TreeNode* child = copyTree(source->firstChild);
    if (child) {
        dest->firstChild = child;
        child->parent = dest;
    }

    TreeNode* next_sibling = copyTree(source->nextSibling);
    if (next_sibling) {
        dest->nextSibling = next_sibling;
        dest->nextSibling->parent = dest->parent;
    }

    return dest;
}

void deleteDirNode(TreeNode** root, TreeNode* deleteNode) {
    if (*root == deleteNode) {
        freeDirTree(root);
    }

    TreeNode* parent = deleteNode->parent;

    TreeNode* child = parent->firstChild;

    if (parent->firstChild == deleteNode) {
        parent->firstChild = deleteNode->nextSibling;

    } else {
        while (1) {
            if (child->nextSibling == deleteNode) {
                child->nextSibling = deleteNode->nextSibling;

                break;
            }

            child = child->nextSibling;
        }
    }

    deleteNode->nextSibling = NULL;

    freeDirTree(&deleteNode);
}

TreeNode* getRootDir() {
    WCHAR root_dir_name[MAX_NAME];

    WCHAR buffer[MAX_NAME];
    fgetws(buffer, sizeof(buffer) / sizeof(WCHAR), stdin);

    // lấy vị trí bắt đầu của path
    WCHAR* path_ptr = wcsstr(buffer, L":\\");
    if (path_ptr == NULL) {
        fwprintf(stderr, L"File Path is invalid!\n");
        exit(EXIT_FAILURE);
    }

    // tìm vị trí đầu tiên
    while (*path_ptr != L' ') {
        path_ptr--;
    }
    path_ptr++;

    path_ptr[wcscspn(path_ptr, L"\r\n")] = L'\0';

    // lấy vị trí bắt đầu của name
    WCHAR* name_ptr = buffer + wcslen(buffer);
    while (*name_ptr != L'\\') {
        name_ptr--;
    }
    name_ptr++;

    TreeNode* root_dir = createDirNode(name_ptr, path_ptr, TRUE);
    createDir(name_ptr);

    return root_dir;
}

TreeNode* buildDirectoryTree() {
    TreeNode* root_dir = getRootDir();
    return root_dir;
}

void freeDirArr(WCHAR** dir_arr, int dir_size) {
    for (int i = 0; i < dir_size; i++) {
        free(dir_arr[i]);
    }

    free(dir_arr);
}


WCHAR** getDirArr(TreeNode* root_dir, int* cur_size) {
    WCHAR** dir_arr = (WCHAR**)malloc(sizeof(WCHAR*));
    *cur_size = 0;
    int max_size = 1;

    TreeNode* child = root_dir->firstChild;

    while (child) {
        dir_arr[(*cur_size)++] = wcsdup(child->name);

        if (*cur_size >= max_size) {
            max_size <<= 1;
            dir_arr = (WCHAR**)realloc(dir_arr, max_size * sizeof(WCHAR*));
        }

        child = child->nextSibling;
    }

    dir_arr = (LPWSTR*)realloc(dir_arr, *cur_size * sizeof(LPWSTR));
    return dir_arr;
}


WCHAR** getDirArrFromXML(FILE* f_ptr, int* cur_size) {
    *cur_size = 0;
    int max_size = 1;

    LPWSTR* dir_arr = (LPWSTR*)realloc(NULL, sizeof(LPWSTR));

    WCHAR buf[MAX_NAME];

    rewind(f_ptr);
    while (fgetws (buf, MAX_NAME, f_ptr)) {
        if (buf[0] == L'\n') {
            break;
        }
        // tìm vị trí mở đầu tiên
        WCHAR* open_ptr = wcsstr(buf, L"<");
        // check xem có phải là con trực tiếp của thư mục tổng không

        //if (open_ptr - buf == INDENT * sizeof(WCHAR)) { là sai vì trừ con trỏ không cần nhân sizeof,
                                                        // nó tự nhảy

        if (open_ptr - buf == INDENT) {
            // là con
            // tìm nháy mở
            WCHAR* open_quotes = wcsstr(buf, L"\"");

            // có thể xảy ra trường hợp tag đóng của folder. VD : </Directory>
            if (open_quotes == NULL) {
                continue;
            }

            // tiếp tục tìm nháy đóng sau nháy mở và gán kết thúc
            WCHAR* close_quotes = wcsstr(open_quotes + 1, L"\"");
            *close_quotes = L'\0';

            // copy vào dir_arr
            dir_arr[(*cur_size)++] = wcsdup(open_quotes + 1);

            if (*cur_size >= max_size) {
                max_size <<= 1;
                dir_arr = (WCHAR**)realloc(dir_arr, max_size * sizeof(WCHAR*));
            }
        }
    }

    dir_arr = (WCHAR**)realloc(dir_arr, *cur_size * sizeof(WCHAR*));
    return dir_arr;
}

void listFileAndFolders(TreeNode* root_dir, DWORD sorted) {

    int cur_size = 0;
    LPCWSTR* dir_arr = getDirArr(root_dir, &cur_size);
    if (sorted == ASC || sorted == DEC) {
        quickSort(dir_arr, 0, cur_size - 1);
    }

    int start, end, dif;


    if (sorted == DEC) {
        start = cur_size - 1;
        end = -1;
        dif = -1;
    }
    else {
        start = 0;
        end = cur_size;
        dif = 1;
    }

    for (int i = start; i != end; i += dif) {
        wprintf(L"%ls\n", dir_arr[i]);
    }

    freeDirArr(dir_arr, cur_size);

}

void listFileAndDirWithPattern(TreeNode* root, LPWSTR pattern) {
    if (root == NULL) { return; }
    if (wcsstr(root->name, pattern)) {
        if (root->isDirectory) {
            wprintf(L"[DIR] %ls\n", root->name);
        } else {
            wprintf(L"[FILE] %ls\n", root->name);
        }
    }

    listFileAndDirWithPattern(root->firstChild, pattern);
    listFileAndDirWithPattern(root->nextSibling, pattern);
}

void addNewDirectoryOrFile(TreeNode* root, LPCWSTR name, BOOL isDirectory) {
    DWORD index = 0;
    wprintf(L"%lu. %ls\n", index++, root->name);

    TreeNode* child = root->firstChild;

    while (child) {
        wprintf(L"%lu. %ls\n", index++, child->name);

        child = child->nextSibling;
    }

    WCHAR buf[MAX_NAME];
    fgetws(buf, MAX_NAME, stdin);
    DWORD chosen_index = wcstol(buf, NULL, 10);
    wprintf(L"%lu\n", chosen_index);

    if (chosen_index == 0) {
        addDirChild(root, name, isDirectory);
        return ;
    } else {
        child = root->firstChild;
        while (--chosen_index) {
            child = child->nextSibling;
        }

        if (child->isDirectory == FALSE) {  // không phải folder thì cho chọn lại
            wprintf(L"This isn't a folder!\n");
            addNewDirectoryOrFile(root, name, isDirectory);
        } else {
            addNewDirectoryOrFile(child, name, isDirectory);
        }
    }
}

// đã được đảm bảo là new không là con cháu của current
// nếu không được đảm bảo thì phải chạy ngược theo con trỏ parent để check
void moveFileOrDir(TreeNode* currentFileOrDir, TreeNode* newDirectory) {
    if (currentFileOrDir == NULL ||
        newDirectory == NULL ||
        currentFileOrDir == newDirectory) {
        return;
    }

    // tách nút con khỏi vị trí hiện tại
    removeDir(currentFileOrDir);

    // thêm nút con vào thư mục mới
    addDir(newDirectory, currentFileOrDir);
}

void copyDirectory(TreeNode* newDirectory, TreeNode* currentDir) {


    TreeNode* copyDir = createDirNode(currentDir->name, currentDir->path, currentDir->isDirectory);
    copyDir->firstChild = copyTree(currentDir->firstChild);


    addDir(newDirectory, copyDir);
}

void copyFile(TreeNode* newDirectory, TreeNode* currentFile) {
    TreeNode* copyFile = createDirNode(currentFile->name, currentFile->path, FALSE);

    addDir(newDirectory, copyFile);
}

TreeNode* buildTree(TreeNode* root_dir, WCHAR* buffer, DWORD prev_indent) {
    if (root_dir == NULL || buffer == NULL || *buffer == L'0') {
        return NULL;
    }


    // check xem có là con của root_dir hay không, nếu không thì backtrack
    DWORD indent = 0;
    WCHAR* ptr = buffer;

    while ((*ptr == L'+' || *ptr == L'-') && *ptr != L'\0') {
        ptr++;
        indent++;
    }

    // hết con, backtrack
    if (indent <= prev_indent) {
        return NULL;
    }

    BOOL is_dir = FALSE;
    if (*buffer == L'+') {
        is_dir = TRUE;
    }

    buffer[wcscspn(buffer, L"\r\n")] = L'\0';

    TreeNode* child = addDirChild(root_dir, ptr, is_dir);

    if (fgetws(buffer, MAX_NAME, stdin) == NULL) {
        *buffer = '\0';
        return NULL;
    }


    while (TRUE) {
        // thêm con của con
        TreeNode* new_child = buildTree(child, buffer, prev_indent + 1);

        // thêm anh chị em của con
        TreeNode* next_child = buildTree(root_dir, buffer, prev_indent);

        if (next_child == NULL) {
            break;
        }
    }

    return child;
}

void printTreeXML(FILE* f_ptr, TreeNode* root_dir, DWORD indent) {
    if (root_dir == NULL) {
        return ;
    }
    WCHAR buf[MAX_NAME] = L"\0";

    for (DWORD i = 0; i < indent; i++) {
        wcscat(buf, L"   ");
    }

    if (root_dir->isDirectory == TRUE) {
        WCHAR close_buf[MAX_NAME];
        // indent như mở tag
        wcscpy(close_buf, buf);

        WCHAR temp[MAX_NAME];
        // ghi tên dir vào tag mở
        swprintf(temp, MAX_NAME,L"<Directory name=\"%ls\">\n", root_dir->name);
        wcscat(buf, temp);
        // ghi tag đóng
        wcscat(close_buf, L"</Directory>\n");

        // mô phỏng chính xác thứ tự, ghi tag mở, hết tag con mới đến tag đóng
        fwprintf(f_ptr, L"%ls", buf);

        TreeNode* child = root_dir->firstChild;
        while (child) {
            printTreeXML(f_ptr, child, indent + 1);
            child = child->nextSibling;
        }

        fwprintf(f_ptr, L"%ls", close_buf);
    } else {
        WCHAR temp[MAX_NAME];
        swprintf(temp, MAX_NAME, L"<File name=\"%ls\"/>\n", root_dir->name);

        wcscat(buf, temp);

        fwprintf(f_ptr, buf);
    }

}

// tạo các array trước và hiện tại
// để dễ dàng ta qsort trước khi so sánh
// O(nlogn) cho sắp xếp
// và O(n) cho việc so sánh các cặp(giống kiểu so sánh để merge trong Merge Sort)
// thay vì O(n^2) khi so sánh từng cặp
void checkTreeXML(FILE* f_ptr, TreeNode* root) {
    // xử lí danh sách của thư mục hiện tại
    int cur_dir_size = 0;
    // lấy các file, folder của thư mục hiện tại
    WCHAR** cur_dir_arr = getDirArr(root, &cur_dir_size);

    if (cur_dir_size == 0) {
        puts("Thư mục hiện tại rỗng!");
    } else {
        quickSort(cur_dir_arr, 0, cur_dir_size - 1);
    }

    // xử lí danh sách thư mục file xml
    int xml_dir_size = 0;
    // lấy các file, folder của thư mục lưu trong file xml
    WCHAR** xml_dir_arr = getDirArrFromXML(f_ptr, &xml_dir_size);

    if (xml_dir_size == 0) {
        puts("Thư mục tại thời điểm save file rỗng!");
    } else {
        quickSort(xml_dir_arr, 0, xml_dir_size - 1);
    }


    // bắt đầu chạy
    int cur_index = 0;
    int xml_index = 0;

    // thằng nào bé hơn thì chắc chắn thằng đó không chứa trong thư mục bên kia
    // vì nó là thằng bé nhất rồi
    while (cur_index < cur_dir_size && xml_index < xml_dir_size) {
        int compare_result = wcscmp(cur_dir_arr[cur_index], xml_dir_arr[xml_index]);

        if (compare_result < 0) {
            wprintf(L"Added folder: %ls\n", cur_dir_arr[cur_index++]);
        } else if (compare_result == 0) {
            cur_index++;
            xml_index++;
        } else {
            wprintf(L"Removed folder: %ls\n", xml_dir_arr[xml_index++]);
        }
    }

    while (cur_index < cur_dir_size) {
        wprintf(L"Added folder: %ls\n", cur_dir_arr[cur_index++]);
    }

    while (xml_index < xml_dir_size) {
        wprintf(L"Removed folder: %ls\n", xml_dir_arr[xml_index++]);
    }


    freeDirArr(cur_dir_arr, cur_dir_size);
    freeDirArr(xml_dir_arr, xml_dir_size);
}




int directoryManager() {

    srand(time(NULL));

    //getCurDir();
    //listFileAndFolders(root_directory, 0);
    //createDir();
    //listFileAndFolders(root_directory, 0);

    //LPCWSTR root_dir_name = L"DatXinhGai";
    //LPWSTR cur_path = getCurPath(root_directory);

    TreeNode* root_dir = buildDirectoryTree();
    //wprintf(L"%ls\n", root_dir->path);

    //listCurrentFileAndFolders(root_dir->path);

// print selection

    static WCHAR buffer[MAX_NAME];
    fgetws(buffer,  MAX_NAME, stdin);

    buildTree(root_dir, buffer, 0);
    //printTree(root_dir, 0);

    LPCWSTR file_name = L"dirXML.txt";
    FILE* f_ptr = _wfopen(file_name, L"w+"); // mở ở chế độ ghi wide character
    if (f_ptr == NULL) {
        fprintf(stderr, "Cannot open file to printXML!\n");
        exit(EXIT_FAILURE);
    }
    listFileAndFolders(root_dir, DEC);

    printTreeXML(f_ptr, root_dir, 0);

    //deleteDirNode(&root_dir, root_dir->firstChild);

    printTree(root_dir, 0);

    addNewDirectoryOrFile(root_dir, L"DatXinhGai", TRUE);
    printTree(root_dir, 0);

    listFileAndDirWithPattern(root_dir, L"xpi");
    printTree(root_dir, 0);

    //moveFileOrDir(root_dir->firstChild, root_dir->firstChild->nextSibling);
    //printTree(root_dir, 0);

    copyDirectory(root_dir->firstChild->nextSibling->nextSibling, root_dir->firstChild);
    printTree(root_dir, 0);

    copyFile(root_dir->firstChild, root_dir->firstChild->nextSibling);
    printTree(root_dir, 0);

    deleteDirNode(&root_dir, root_dir->firstChild->nextSibling);

    checkTreeXML(f_ptr, root_dir);

    freeDirTree(&root_dir);


    fclose(f_ptr);
    return 0;
}
