//
// Created by mengdongqi on 15-11-20.
//
#include <stdio.h>
#include <stdlib.h>
#include "approx_vc1.h"

void read_and_handle_input(USER_INPUT *user_input) {
    size_t line_buffer_max = LINE_BUFFER_SIZE;
    char *line = (char *) malloc(line_buffer_max);
    bool getV = false, getE = false;
    while (getline(&line, &line_buffer_max, stdin) != EOF) {
        int nodeNum = 0;
        if (line[0] == 'V') {
            clear_user_input(user_input);
            sscanf(line + 2, "%d", &nodeNum);
            if (DEBUG)
                printf("catch input node Num: %d\n", nodeNum);
            if (nodeNum > 0) {
                user_input->input_node_num = nodeNum;
                getV = true;
            } else {
                printf("Error: node Num %d should be greater than 0.\n", nodeNum);
                clear_user_input(user_input);
                continue;
            }
        }
        //printf("V:%d", user_input->input_node_num);
        if (line[0] == 'E') {
            bool input_edge_legal = true;
            int a, b = 0;
            char s[32]; // max num of 32 digit
            char *line_cursor = (line + 3);
            sscanf(line_cursor, "<%d,%d>", &a, &b);
            line_cursor += num_str_length(s, a) + num_str_length(s, b) + 3;
            EDGE e = create_input_edge(user_input, a, b);
            if (check_input_edge(e, user_input->input_node_num) == false) {
                input_edge_legal = false;
            }
            if (DEBUG)
                printf("catch input edge between a=%d and b=%d\n", a, b);
            while (*line_cursor != '}') {
                sscanf(line_cursor, ",<%d,%d>", &a, &b);
                line_cursor += num_str_length(s, a) + num_str_length(s, b) + 4;
                if (DEBUG)
                    printf("catch input edge between a=%d and b=%d\n", a, b);
                EDGE e = create_input_edge(user_input, a, b);
                if (check_input_edge(e, user_input->input_node_num) == false) {
                    input_edge_legal = false;
                    break;
                }
            }
            if (input_edge_legal == false) {
                clear_user_input(user_input);
                continue;
            } else {
                getE = true;
            }
        }
        // printf("E num:%d", user_input->input_edge_num);

        if (!(getV && getE)) continue;
        if (getV && getE) {


        }
    }
    free(line);
}

void count_nodes_inciden(USER_INPUT *user_input, int *node_inciden) {
    int edge_count = user_input->input_edge_num, node_count = user_input->input_node_num, i;
    for (i = 0; i < edge_count; i++) {
        node_inciden[user_input->input_edges[edge_count].A]++;
        node_inciden[user_input->input_edges[edge_count].B]++;
    }
}

void get_min_vertixs(int *node_inciden,USER_INPUT *user_input,int min_vertixs){
    int node_inciden_sort[user_input->input_node_num];
    EDGE edge_include[user_input->input_edge_num];
    copyArray(node_inciden,node_inciden_sort,user_input->input_node_num);
    qsort(node_inciden_sort, user_input->input_node_num, sizeof(node_inciden_sort[0]), compare_num);
    while(){

    }
}

int compare_num(const void *num_A, const void *num_B) {
    return *(int *) num_A - *(int *) num_B;

}

void copyArray(int arrayOrigine[], int arrayCopy[], int arraySize) {
    int i;
    for (i = 0; i <= arraySize; i++) {
        arrayCopy[i] = arrayOrigine[i];
    }
}