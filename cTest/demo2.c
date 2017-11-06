//
// Created by mengdongqi on 15-10-30.
//
#include <stdio.h>
#include "demo2.h"

int main() {
    long cc, cl, cw;
    int c, state;
    while ((c = getchar()) != EOF) {
        cc++;
        if (c == '\n')
            cl++;
        if (c == ' ' || c == '\t' || c == '\n')
            state = OUT;
        else if (state == OUT) {
            state = IN;
            cw++;
        }

    }
    printf("%ld,%ld,%ld", cc, cl, cw);
    return 0;
}