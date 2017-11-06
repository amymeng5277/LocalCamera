//
// Created by mengdongqi on 15-10-24.
//
#include "base.h"

#ifndef ASSIGNMENT2_GRAPH_H
#define ASSIGNMENT2_GRAPH_H

// adjacency list node
typedef struct AdjListNode *pAdjListNode;
typedef struct AdjListNode ADJ_LIST_NODE;
struct AdjListNode {
    int index; // index of this adj_List
    pAdjListNode next;
};


// an adjacency list
struct AdjList {
    pAdjListNode head;  // pointer to head node of list
};
typedef struct AdjList *pAdjList;
typedef struct AdjList ADJ_LIST;

// Graph definition.
// A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
#define MAX_INPUT_NODE_NUM 50 // user could only input less than 100 node of these edges
struct Graph {
    int vertices_from_input_edges[MAX_INPUT_NODE_NUM];
    int visited[MAX_INPUT_NODE_NUM]; // mark if this vertex is visited
    int v_num; // vertices number
    pAdjList array;
};
typedef struct Graph GRAPH;
typedef struct Graph *pGraph;

// create a new adjacency list node
pAdjListNode new_AdjListNode(int index);

//creates a graph of v_num vertices
pGraph create_graph(USER_INPUT *user_input);

// Adds an edge to an undirected graph
void add_edge(pGraph graph, int src_index, int destination_index);

// print the adjacency list representation of graph
void print_graph(pGraph graph);

void convert_input_edges_into_vertices(USER_INPUT *user_input, pGraph graph);

void populate_edges_into_graph(USER_INPUT *user_input, pGraph graph);

// get vertex index of vertices generated from input edges
int get_vertex_index_from_input_edges(int value, pGraph graph);

// BFS traverse from start_index to end_index
// if path found return true, else return false
// if path found, stop the traverse and print the path, and return true
bool bfs_traverse(int start_index, int end_index, pGraph graph);

//get first vertex from an adjacency list start from index
int first_vertex(pGraph graph, int index);

//get next vertex_index from an adjacency list start from index
//return next vertex_index after 'before_index'
int next_vertex(pGraph graph, int start_index, int before_index);

// print path from node
void print_path(pGraph graph, pNODE node);

// clear this graph
void clear_graph(pGraph graph);

#endif //ASSIGNMENT2_GRAPH_H
