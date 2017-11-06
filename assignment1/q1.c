//
// Created by mengdongqi on 16-01-24.
//

#include "q1.h"

void main() {
    int a;
    a = mod(-5, 2);
    printf("%d\n", a);
}

int mod(int divined, int divisor) {
    int res = divined % divisor;
    return (res > 0) ? res : (res + divisor);
}