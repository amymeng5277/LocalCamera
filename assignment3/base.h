//
// Created by mengdongqi on 15-11-05.
//

#ifndef CTEST_BASE_H
#define CTEST_BASE_H

#define BUFSIZ 1024
#define FILENAME "/dev/random"
struct Node {
    int x;
    int y;
};
typedef struct Node NODE;
typedef struct Node *pNodeList;

int randnum(int upper, int lower);

NODE generate_node(int x_upper, int x_lower, int y_upper, int y_lower);

pNodeList generate_multi_nodes(int x_upper, int x_lower, int y_upper, int y_lower, int count);

#endif //CTEST_BASE_H
