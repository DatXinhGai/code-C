//
// Created by HP LAPTOP on 04/03/2025.
//
#include <stdio.h>
#include <stdlib.h>

// lưu prev và pos
// khí tăng thì add vào stack và prev = pos = i + 1
// khi giảm thì chạy ngược đến khi nào mà tìm được thằng bé hơn hoặc dãy rỗng thì thôi
// khi đó cập nhật pos = pos thằng trước + 1
// còn prev thì lấy cái của nó ở dãy h để tí truy xuất value
/*
typedef struct {
int prev;
int pos;
int value;
} STACK;

long largestRectangle(int h_count, int* h) {
long re = 0;

STACK* stack = (STACK*)malloc(sizeof(STACK));
int stack_count = 0;
int stack_max = 1;

for (int i = 0; i < h_count; i++) {
if (!stack_count || stack[stack_count - 1].value < h[i]) {
stack[stack_count].prev = i;
stack[stack_count].value = h[i];
stack[stack_count].pos = i;
} else {
while (stack_count && stack[stack_count - 1].value >= h[i]) {
long s = stack[stack_count - 1].value*(i - stack[stack_count - 1].pos);
if (s > re) {
re = s;
}
stack_count--;
}
stack[stack_count].prev = i;
if (stack_count) {
stack[stack_count].pos = stack[stack_count - 1].prev + 1;
} else {
stack[stack_count].pos = 0;
}
stack[stack_count].value = h[i];
}
stack_count++;
if (stack_count >= stack_max) {
stack_max <<= 1;
stack = (STACK*)realloc(stack, stack_max*sizeof(STACK));
}
}

for (int i = 0; i < stack_count; i++) {
long s = (h_count - stack[i].pos)*stack[i].value;
if (s > re) {
re = s;
}
}
printf("%ld\n", re);
return re;
}
*/



long largestRectangle(int h_count, int* h) {
    int* stack = (int*)malloc(h_count*sizeof(int));
    int maxArea = 0;

    int top = -1;

    for (int i = 0; i <= h_count; i++) {
        int hei = (i == h_count)? 0 : h[i];

        while (top != - 1 && h[stack[top]] > hei) {
            int height = h[stack[top--]];
            int width = (top == -1)? i : i - stack[top] - 1;
            long area = (long) height * width;
            if (area > maxArea) { maxArea = area; }
        }

        stack[++top] = i;
    }

    free(stack);
    return maxArea;

}