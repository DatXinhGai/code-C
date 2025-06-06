//
// Created by HP LAPTOP on 27/03/2025.
//
#include <stdio.h>
#include <stdlib.h>



#define SIZE 10

// lưu tích
int row_prod[SIZE] = {0};
int col_prod[SIZE] = {0};
// check xem hàng bị chiếm bao nhiêu chỗ rồi
int row_check[SIZE] = {0};
int col_check[SIZE] = {0};
// lưu value trước đó của hàng
int row_value[SIZE];
int col_value[SIZE];
// check xem 1 số có là ước của tích


int mindien(int a, int b) {
	return (a < b)? a : b;
}

void divisorsFind(int** isDivisor, int a) {
	*isDivisor = (int*)calloc((a + 1), sizeof(int));
	for (int i = 1; i <= a; i++) {
		if (a%i == 0) {
			(*isDivisor)[i] = 1;
		}
	}
}

int numBoard(int** grid, int size, int** isRowDivisor, int** isColDivisor,  int row, int col, int count) {

	if (count == 2*size) {
		/*
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				printf("%d ", grid[i][j]);
			}
			puts("");
		}
		*/
		return 1;
	}
	// điền đủ giá trị
	if (count == (size << 1)) {
		return 1;
	}	// vượt quá số hàng
	if (row == size) {
		return 0;
	}

	// hàng hoặc cột đã bị chiếm
	if (row_check[row] == 2 || col_check[col] == 2) {
		if (numBoard(grid, size, isRowDivisor, isColDivisor, row + (col + 1)/size, (col + 1)%size, count)) {
			return 1;
		}
	} else if (row_check[row] == 0 && col_check[col] == 0) {	// chưa bị chiếm
		row_check[row]++;
		col_check[col]++;
		for (int i = 1; i <= mindien(row_prod[row], col_prod[col]); i++) {

			if (isRowDivisor[row][i] && isColDivisor[col][i]) {

				row_value[row] = i;
				col_value[col] = i;
				grid[row][col] = i;

				if (numBoard(grid, size, isRowDivisor, isColDivisor, row + (col + 1)/size, (col + 1)%size, count + 1)) {
					return 1;
				}
			}
		}

		// nếu không valid
		row_check[row]--;
		col_check[col]--;
		grid[row][col] = 0;
	} else {
		int val;
		int resume = 0;
		if (row_check[row] && col_check[col]) {
			val = row_prod[row]/row_value[row];
			if (val != col_prod[col]/col_value[col]) {
				resume = 1;
			}
		} else if (row_check[row]) {
			val = row_prod[row]/row_value[row];
			if (!isColDivisor[col][val]) {
				resume = 1;
			}
			col_value[col] = val;
		} else if (col_check[col]) {
			val = col_prod[col]/col_value[col];
			if (!isRowDivisor[row][val]) {
				resume = 1;
			}
			row_value[row] = val;
		}

		if (!resume) {
			row_check[row]++;
			col_check[col]++;
			grid[row][col] = val;
			if (numBoard(grid, size, isRowDivisor, isColDivisor, row + (col + 1)/size, (col + 1)%size, count + 1)) {
				return 1;
			}

			row_check[row]--;
			col_check[col]--;
			grid[row][col] = 0;
		}
		if (numBoard(grid, size, isRowDivisor, isColDivisor, row + (col + 1)/size, (col + 1)%size, count)) {
			return 1;
		};
	}
	return 0;
}

int dien() {

	int size;
	scanf("%d", &size);
	int** isRowDivisor = (int**)malloc(SIZE*sizeof(int*));
	int** isColDivisor = (int**)malloc(SIZE*sizeof(int*));
	for (int j = 0; j < size; j++) {
		scanf("%d", &col_prod[j]);
		divisorsFind(&isColDivisor[j], col_prod[j]);

	}

	for (int i = 0; i < size; i++) {
		scanf("%d", &row_prod[i]);
		divisorsFind(&isRowDivisor[i], row_prod[i]);
	}

	int** grid = (int**)malloc(size*sizeof(int*));
	for (int i = 0; i < size; i++) {
		grid[i] = (int*)calloc(size, sizeof(int));
	}



	numBoard(grid, size, isRowDivisor, isColDivisor, 0, 0, 0);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			printf("%2d ",grid[i][j]);
		}
		puts("");
	}

	return 0;
}