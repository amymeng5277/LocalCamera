//
// Created by mengdongqi on 15-10-24.
//

#include <stdlib.h>
#include <stdio.h>
#include "base.h"
#include "util.h"
#include "graph.h"

pNODE create_node(int node_id, pNODE parentNodeInBFSTree) {
    pNODE node = (pNODE) malloc(sizeof(NODE));
    node->node_id = node_id;
    node->parentNodeInBFSTree = parentNodeInBFSTree;
    node->nextNodeInQueue = NULL;
    return node;
}

EDGE create_input_edge(USER_INPUT *user_input, int A, int B) {
    EDGE edge = {A, B};
    user_input->input_edge_num++;
    user_input->input_edges[user_input->input_edge_num - 1] = edge;
    return edge;
}

// check if node of input edge is in the range of [0,input_node_num)
bool check_input_edge(EDGE e, int input_node_num) {
    if ((e.A >= 0 && e.A <= input_node_num - 1) &&
        (e.B >= 0 && e.B <= input_node_num - 1))
        return true;
    printf("Error: the edge of %d -> %d is not in the range of [0,%d)\n", e.A, e.B, input_node_num);
    return false;
}

bool check_node_in_query_s(int start, int end, int input_node_num) {
    if ((start >= 0 && start <= input_node_num - 1) &&
        (end >= 0 && end <= input_node_num - 1))
        return true;
    printf("Error: the start node: %d or end node: %d of query s is not in the range of [0,%d)\n", start, end, input_node_num);
    return false;
}

// check if start or end node in the edge list from user input
bool check_node_in_query_s_in_graph(int start, int end, USER_INPUT *user_input) {
    bool start_flag = false;
    bool end_flag = false;
    for (int i = 0; i < user_input->input_edge_num; i++) {
        if (user_input->input_edges[i].A == start || user_input->input_edges[i].B == start)
            start_flag = true;
        if (user_input->input_edges[i].A == end || user_input->input_edges[i].B == end)
            end_flag = true;
        if (start_flag && end_flag) // if found in the user input edge list
            return true;
    }
    printf("Error: the start node: %d or end node: %d of query s is not in the user input edges.\n", start, end);
    return false;
}

void read_and_handle_input(USER_INPUT *user_input) {
    size_t line_buffer_max = LINE_BUFFER_SIZE;
    char *line = malloc(line_buffer_max);
    while (getline(&line, &line_buffer_max, stdin) != EOF) {
        int nodeNum = 0;
        if (line[0] == 'V') {
            clear_user_input(user_input);
            sscanf(line + 2, "%d", &nodeNum);
            if (DEBUG)
                printf("catch input node Num: %d\n", nodeNum);
            if (nodeNum > 0) {
                user_input->input_node_num = nodeNum;
            } else {
                printf("Error: node Num %d should be greater than 0.\n", nodeNum);
                clear_user_input(user_input);
                continue;
            }
        }
        if (line[0] == 'E') {
            bool input_edge_legal = true;
            int a, b = 0;
            int a_length, b_length = 0;
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
            }
        }
        if (line[0] == 's') {
            int start, end;
            sscanf(line, "s %d %d", &start, &end);
            if (DEBUG)
                printf("query s: start=%d, end=%d\n", start, end);
            if (check_node_in_query_s(start, end, user_input->input_node_num) == false) {
//                clear_user_input(user_input);
                continue;
            }
            if (start == end) {
                printf("%d-%d\n", start, end); // node to itself
            } else {
                if (check_node_in_query_s_in_graph(start, end, user_input) == false) {
//                    clear_user_input(user_input);
                    continue;
                } else {
                    pGraph graph = create_graph(user_input);
                    if (DEBUG)
                        print_graph(graph);
                    int start_index = get_vertex_index_from_input_edges(start, graph);
                    int end_index = get_vertex_index_from_input_edges(end, graph);
                    bool exist_path = bfs_traverse(start_index, end_index, graph);
                    if (!exist_path) {
                        printf("Error: no path between vertex %d and vertex %d.\n", start, end);
                    }
                    clear_graph(graph);
                }
            }

        }
    }
    free(line);
}

void clear_user_input(USER_INPUT *user_input) {
    user_input->input_node_num = 0;
    user_input->input_edge_num = 0;
    for (int i = 0; i < MAX_INPUT_EDGE_NUM; i++) {
        EDGE e = {0, 0};
        user_input->input_edges[i] = e;
    }
    user_input->start_node = 0;
    user_input->end_node = 0;
}
