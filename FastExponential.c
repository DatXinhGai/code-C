//
// Created by HP LAPTOP on 26/02/2025.
//
#include <stdio.h>
#include <stdlib.h>

long long FastExponentialRecurse(int base, int pow) {
    if (pow == 0) {
        return 1;
    }
    return ((pow%2 == 1)? base : 1) * FastExponentialRecurse(base, pow/2) * FastExponentialRecurse(base, pow/2);
}

long long FastExponentialRecurse2(int base, int pow) {
    if (pow == 0) {
        return 1;
    }
    long long temp = FastExponentialRecurse2(base, pow/2);

    return temp * temp * ((pow%2 == 1)? base : 1);
}

int FastExponential() {
    int base, pow;
    scanf("%d%d", &base, &pow);

    long long  re = FastExponentialRecurse2(base, pow);

    printf("%lld", re);

    return 0;
}