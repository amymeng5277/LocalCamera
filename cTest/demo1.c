//
// Created by mengdongqi on 15-10-30.
//


#include <stdio.h>
#include <stdlib.h>
#include "demo1.h"

int main() {
    float fahrenheit, centigrade;
    int upper;
    fahrenheit = LOWER;
    printf("Please input a fahrenheit scale\n");
    scanf("%d", &upper);
    while (1) {
        printf("%10s%20s\n", FAHRENHEIT, CENTIGRADE);
        while (fahrenheit <= upper) {
            centigrade = (fahrenheit - 32) * 5 / 9;
            printf("%10.0f%20.1f\n", fahrenheit, centigrade);
            fahrenheit += STEP;
        }
        return 0;
    }
}


void test() {
    printf("I love you");
}