//
// Created by mengdongqi on 15-11-05.
//

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "demo8.h"

main(int argc, char *argv[]) {
    int c, n;
    char myRandomData[BUFSIZ];
    while (--argc > 0 && *++argv[0] == '-') {
        while (c = *++argv[0]) {
            switch (c) {
                case 'c':
                    n = *++argv[0];
                    reasRanmNum(myRandomData, n);
                case 'l':
                    return 0;
                case 'n':
                    return 0;
                case 's':
                    return 0;

            }
        }
    }
}

void reasRanmNum(char myRandomData[], int scale) {
    int randomData = open(FILENAME, O_RDONLY);
    size_t randomDataLen = 0;
    while (randomDataLen < sizeof myRandomData) {
        ssize_t result = read(randomData, myRandomData + randomDataLen, (sizeof myRandomData) - randomDataLen);
        if (result < 0) {
            printf("Error: unable to read %s", FILENAME); // error, unable to read /dev/random
        }
        randomDataLen += result;
    }
    close(randomData);
    int i = 0;
    while (i++ < BUFSIZ) {
        myRandomData[i] = myRandomData[i] % scale;
    }


}

void generateSegment(char myRandomData[],int num){
    rand();
}