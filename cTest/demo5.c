//
// Created by mengdongqi on 15-10-31.
//

#include "demo5.h"
#include <stdio.h>

int main() {
    char array1[20] = {'s', 'i', 'l', 'y', 'h', '\0'};
    char array2[20] = {'s', 'i', 'l', 'o', 'v', 'e', 'y', '\0'};
    squize(array1, array2);
    int i;
    i = 0;
    while (array2[i++] != '\0') {
        printf("%c", array2[i]);
    }
    return 0;
}

void squize(char array1[], char array2[]) {
    /*Cut the same element for array2 */
    int i, j, k;
    j = k = 0;
    while (array2[j] != '\0') {
        char array2Elm = array2[j];
        i = 0;
        while (array1[i++] != '\0') {
            if (array1[i] == array2Elm) {
                break;
            }
        }
        if (array1[i] == '\0') {
            array2[k++] = array2Elm;
        }
        j++;
    }
    array2[k] = '\0';
}