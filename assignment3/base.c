//
// Created by mengdongqi on 15-11-05.
//

#include "base.h"
#include "util.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int randnum(int upper, int lower) {
    char myRandomData[BUFSIZ];
    int randomData = open(FILENAME, O_RDONLY);
    size_t randomDataLen = 0;
    while (randomDataLen < sizeof(myRandomData)) {
        ssize_t result = read(randomData, myRandomData + randomDataLen, (sizeof(myRandomData)) - randomDataLen);
        if (result < 0) {
            char s[MAXLINE];
            sprintf(s, "Error: unable to read %s", FILENAME); // error, unable to read /dev/random
            err_msg1(s);
        }
        randomDataLen += result;
    }
    close(randomData);
    int scale;
    scale = upper - lower + 1;
    unsigned c = myRandomData[0];
    int s = c % scale + lower;
    return s;

}

NODE generate_node(int x_upper, int x_lower, int y_upper, int y_lower) {
    int x = randnum(x_upper, x_lower);
    int y = randnum(y_upper, y_lower);
    NODE node;
    node.x = x;
    node.y = y;
    return node;
}

pNodeList generate_multi_nodes(int x_upper, int x_lower, int y_upper, int y_lower, int count) {
    pNodeList nodeList = (pNodeList) malloc(count * sizeof(NODE));
    int i = 0;
    while (i < count) {
        nodeList[i] = generate_node(x_upper, x_lower, y_upper, y_lower);
        i++;
    }
    return nodeList;
}

