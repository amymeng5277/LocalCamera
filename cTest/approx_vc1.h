//
// Created by mengdongqi on 15-11-20.
//

#ifndef ASSIGNMENT4_APPROX_VC1_H
#define ASSIGNMENT4_APPROX_VC1_H

#define DEBUG false     //True means program will print debug information
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

#define MAX_INPUT_EDGE_NUM 100 // user could only input less than 100 edges
typedef struct Edge EDGE;
typedef struct UserInput {
    int input_node_num;
    int input_edge_num;
    EDGE input_edges[MAX_INPUT_EDGE_NUM];

} USER_INPUT;

#define LINE_BUFFER_SIZE 1024*10 // one line max character numbers


void read_and_handle_input(USER_INPUT *user_input);

// deal with information
void clear_user_input(USER_INPUT *user_input);

//clear user input
bool check_input_edge(EDGE e, int input_node_num);

// check is eage format legale
EDGE create_input_edge(USER_INPUT *user_input, int A, int B);

// check if node of input edge is in the range of [0,input_node_num)
int num_str_length(char *s, int num);

void count_nodes_inciden(USER_INPUT *user_input, int *node_inciden);

void get_min_vertixs(int *node_inciden,USER_INPUT *user_input,int min_vertixs);

int compare_num(const void *num_A, const void *num_B);

void copyArray(int arrayOrigine[], int arrayCopy[], int arraySize);

#endif //ASSIGNMENT4_APPROX_VC1_H

