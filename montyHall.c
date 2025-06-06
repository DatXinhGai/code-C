//
// Created by HP LAPTOP on 26/03/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIMULATION 1000000

int decision(int switched) {
    int car_door = rand() % 3;
    int origin_door = rand() % 3;

    int monty_door;
    do {
        monty_door = rand() % 3;
    } while (monty_door == car_door || monty_door == origin_door);

    int last_door = origin_door;
    if (switched) {
        do {
            last_door = rand() % 3;
        } while (last_door == origin_door || last_door == monty_door);
    }

    return last_door == car_door;
}

int montyHall() {
    srand(time(NULL));
    int origin_true = 0;
    int switch_true = 0;

    for (int i = 0; i < SIMULATION; i++) {
        origin_true += decision(0);
        switch_true += decision(1);
    }

    printf("origin: %d\n", origin_true);
    printf("switch: %d\n", switch_true);
    printf("Rates Of Switch: %lf", switch_true*1.0/SIMULATION);

    return 0;
}