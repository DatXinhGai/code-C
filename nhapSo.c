//
// Created by HP LAPTOP on 29/12/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "readline.h"






void nhapSo() {
    char *n_endptr;
    char *n_str = readline();
    int n = strtol(n_str, &n_endptr, 10);
    if (n_str == n_endptr || *n_endptr == '\0') {
        exit(EXIT_FAILURE);
    }
}