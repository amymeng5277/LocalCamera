//
// Created by mengdongqi on 15-10-24.
//

#include <stdbool.h>

#ifndef ASSIGNMENT2_BASE_H
#define ASSIGNMENT2_BASE_H
#define DEBUG false

typedef struct Node NODE;
typedef struct Node *pNODE;

struct Node {
    pNODE nextNodeInQueue;
    pNODE parentNodeInBFSTree;
    int node_id; // node id, vary from 0 to node_num
};

pNODE create_node(int node_id, pNODE parentNodeInBFSTree);

// record user input
struct Edge {
    int A;
    int B;
};
typedef struct Edge EDGE;

#define MAX_INPUT_EDGE_NUM 100 // user could only input less than 100 edges
typedef struct UserInput {
    int input_node_num;
    int input_edge_num;
    EDGE input_edges[MAX_INPUT_EDGE_NUM];
    int start_node, end_node; // query of s
} USER_INPUT;

EDGE create_input_edge(USER_INPUT *user_input, int A, int B);

// check if node of input edge is in the range of [0,input_node_num)
bool check_input_edge(EDGE e, int input_node_num);

bool check_node_in_query_s(int start, int end, int input_node_num);

#define LINE_BUFFER_SIZE 1024*10 // one line max character numbers

//read the user input
void read_and_handle_input(USER_INPUT *user_input);

// clear the user input
void clear_user_input(USER_INPUT *user_input);

#endif //ASSIGNMENT2_BASE_H
