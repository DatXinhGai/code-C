//
// Created by HP LAPTOP on 28/02/2025.
//
/*
 #include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// CHƯƠNG TRÌNH TÌM CHỮ XUẤT HIỆN TRONG MA TRẬN


typedef struct {
	char* str;
	int hang;
	int cot;
	int delta_x;
	int delta_y;
} QUERY;

char* readline() {
	int data_len = 0;
	int alloc_len = 1024;
	char* data = (char*)malloc(alloc_len);

	while (true) {
		char* cursor = data + data_len;
		char* line = fgets(cursor, alloc_len - data_len, stdin);
		if (!line) {
			break;
		}

		data_len += strlen(line);
		if (data_len < alloc_len || data[data_len - 1] == '\n') {
			break;
		}

		alloc_len <<= 1;
		char* temp = (char*)realloc(data, alloc_len);
		if (!temp) {
			break;
		}
		data = temp;
	}

	if (data[data_len - 1] == '\n') {
		data_len--;
	}
	while (data[data_len - 1] == ' ') {
		data_len--;
	}
	data[data_len] = '\0';
	data = (char*)realloc(data, data_len + 1);
	return data;
}

bool checkHuong(QUERY* query, char** grid, int row, int col, int x, int y, int delta_x, int delta_y, int dem) {
	if (x < 0 || x >= row || y < 0 || y >= col) {
		return false;
	}

	if (delta_x == 0 && delta_y == 0) {
		return false;
	}

	if (query->str[dem] == grid[x][y]) {
		dem++;
		if (dem == strlen(query->str)) {
			query->delta_x = delta_x;
			query->delta_y = delta_y;
			return true;
		}
		x += delta_x;
		y += delta_y;
		return checkHuong(query, grid, row, col, x, y, delta_x, delta_y, dem);
	}

	return false;
}

bool check(QUERY *query, char** grid, int row, int col) {
	for (int x = 0; x < row; x++) {
		query->hang = x;
		for (int y = 0; y < col; y++) {
			query->cot= y;
			for (int delta_x = -1; delta_x <= 1; delta_x++) {
				for (int delta_y = -1; delta_y <= 1; delta_y++) {
				// xét các hướng dùng vòng lặp
				// gặp 1 hướng được thì ngừng đệ quy
					if (checkHuong(query, grid, row, col, x, y, delta_x, delta_y, 0)) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

int main() {
	int row, col;
	char temp[1024];
	fgets(temp, 1023, stdin);
	char* end_ptr;
	row = strtol(temp, &end_ptr, 10);
	col = strtol(end_ptr, NULL, 10);

	char **grid = (char**)malloc(row*sizeof(char*));
	for (int i = 0; i < row; i++) {
		grid[i] = (char*)malloc(col*sizeof(char));
		char* tam = readline();
		char* ptr = tam;
  		while (*ptr == ' ') {
			ptr++;
		}
		int dem = 0;
		while (*ptr) {
			if (*ptr != ' ') {
				grid[i][dem++] = *ptr;
			}
			ptr++;
		}
	}

	int dem = 0;
	int max = 32;
	QUERY* query = (QUERY*)malloc(max*sizeof(QUERY));
	while (true) {
		char* temp = readline();
		if (temp[0] == '#') {
			break;
		}
		query[dem++].str = temp;
		if (dem >= max) {
			max <<= 1;
			query = (QUERY*)realloc(query, max*sizeof(QUERY));
		}
	}

	for (int i = 0; i < dem; i++) {
		if (check(&query[i], grid, row, col)) {
			printf("%s %d %d %d %d\n", query[i].str, query[i].hang, query[i].cot, query[i].delta_x, query[i].delta_y);
		} else {
			printf("%s -1\n", query[i].str);
		}
	}

	for (int i = 0; i < row; i++) {
		free(grid[i]);
		grid[i] = NULL;
	}
	free(grid);
	grid = NULL;

	for (int i = 0; i < dem; i++) {
		free(query[i].str);
		query[i].str = NULL;
	}
	free(query);
	query = NULL;
	return 0;

}
*/