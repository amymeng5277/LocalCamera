//
// Created by mengdongqi on 15-10-30.
//

#include <stdio.h>

int main() {
    int num[10], i, c, cSpace, cLine, cAnother;
    cSpace = cLine = cAnother = 0;
    for (i = 0; i < sizeof(num)/sizeof(int); i++) {
        num[i] = 0;
    }
    while ((c = getchar()) != EOF) {
        if (c == ' ' || c == '\t')
            cSpace++;
        else if (c == '\n')
            cLine++;
        else if (c >= '0' && c <= '9')
            num[c - '0'] = num[c - '0'] + 1;
        else
            cAnother++;

    }
    printf("White space is %d\n", cSpace);
    printf("It have %d lines\n", cLine);
    printf("Numbers : ");
    for (i = 0; i < sizeof(num)/sizeof(int); i++) {
        printf("%d\t", num[i]);
    }
    return 0;
}