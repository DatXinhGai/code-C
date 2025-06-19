//
// Created by HP LAPTOP on 11/06/2025.
//
#include <stdio.h>
#include <windows.h>

#define UNICODE
#define _UNICODE

int win32_find_data() {
    // nên truyền thêm cả W để phân biệt với A(ASCII)
    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW(L"C:\\Users\\*", &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        wprintf(L"Không thể mở thư mục\n");
        return 1;
    }

    do {
        wprintf(L"%ls\n", findData.cFileName);
    } while (FindNextFileW(hFind, &findData));

    return 0;
}