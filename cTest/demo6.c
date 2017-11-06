//
// Created by mengdongqi on 15-11-03.
//

#include <stdio.h>
#include "demo6.h"

int main() {
    int arrat1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int num = 4;
    int length = sizeof(arrat1) / sizeof(int);
    int indexnum = halfSearch(arrat1, num, length);
    printf("The index you research for %d is %d", num, indexnum);
    return 0;
}

int halfSearch(int orderArray[], int num, int length) {
    int high, low, mid;
    high = length;
    low = 0;
    mid = (high + low) / 2;
    while (orderArray[mid] != num) {
        if (num > orderArray[mid]) {
            low = mid;
            mid = (high + low) / 2;
        } else if (num < orderArray[mid]) {
            high = mid;
            mid = (high + low) / 2;
        } else {
            break;
        }
    }
    return mid;
}