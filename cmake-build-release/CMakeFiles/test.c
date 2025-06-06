//
// Created by HP LAPTOP on 04/01/2025.
//
#include <stdio.h>
#include <stdlib.h>
char* thucHienDich(char* s) {
    s += 1;
    return s;
}

void dichConTro() {
    char *s = malloc(100);
    fgets(s, 100, stdin);
    s = thucHienDich(s);
    puts(s);

}