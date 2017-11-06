//
// Created by mengdongqi on 15-10-30.
//


#include <stdio.h>
#include <stdlib.h>
#include "demo4.h"

int main() {
    char arrayInput[MAXSIZE];
    char arrayMax[MAXSIZE];
    int maxsize, c, i;
    maxsize = getOneLine(arrayInput);
    copyArray(arrayInput, arrayMax, maxsize);
    while ((c = getchar()) != EOF) {
        int tempSize;
        tempSize = getOneLine(arrayInput);
        if (tempSize > maxsize) {
            maxsize = tempSize;
            copyArray(arrayInput, arrayMax, maxsize);

        }
    }
    for (i = 1; i <= maxsize; i++) {
        printf("%c", arrayMax[i]);
    }
    printf("\n");
    return 0;
}

int getOneLine(char array[]) {
    int c, i;
    array[0] = 0;
    i = 1;
    while ((c = getchar()) != '\n' && c != EOF) {
        array[i] = c;
        array[0] = array[0] + 1;
        i++;
    }
    return array[0];
}

void copyArray(char arrayOrigine[], char arrayCopy[], int arraySize) {
    int i;
    for (i = 0; i <= arraySize; i++) {
        arrayCopy[i] = arrayOrigine[i];
    }
}