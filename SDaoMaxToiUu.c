//
// Created by HP LAPTOP on 28/01/2025.
//
#include <stdio.h>
#include <stdlib.h>

// Hàm tìm gốc tập hợp (Find) với path compression
int find(int parent[], int x) {
    if (parent[x] != x) {
        parent[x] = find(parent, parent[x]);  // Path compression
    }
    return parent[x];
}

// Hợp nhất hai tập hợp (Union) với union by rank
void unionSets(int parent[], int rank[], int x, int y) {
    int rootX = find(parent, x);
    int rootY = find(parent, y);
    if (rootX != rootY) {
        if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
}

int SDaoMaxToiUu() {
    int row, col;
    scanf("%d%d", &row, &col);

    int grid[row][col];
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            scanf("%d", &grid[i][j]);
        }
    }

    // Khởi tạo Union-Find
    int size = row * col;
    int parent[size], rank[size];
    for (int i = 0; i < size; i++) {
        parent[i] = i;  // Mỗi ô là một tập hợp riêng
        rank[i] = 0;    // Rank ban đầu bằng 0
    }

    // Duyệt toàn bộ bảng và hợp nhất các ô đất liền
    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, -1, 0, 1};
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (grid[i][j] == 1) {
                for (int d = 0; d < 4; d++) {
                    int ni = i + dx[d];
                    int nj = j + dy[d];
                    if (ni >= 0 && ni < row && nj >= 0 && nj < col && grid[ni][nj] == 1) {
                        unionSets(parent, rank, i * col + j, ni * col + nj);
                    }
                }
            }
        }
    }

    // Đếm số tập hợp gốc (islands)
    int* isIsland = (int*)calloc(size, sizeof(int));
    int numIslands = 0;
    for (int i = 0; i < size; i++) {
        if (grid[i / col][i % col] == 1) {
            int root = find(parent, i);
            if (!isIsland[root]) {
                isIsland[root] = 1;
                numIslands++;
            }
        }
    }

    printf("%d\n", numIslands);

    free(isIsland);
    return 0;
}
