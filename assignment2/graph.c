//
// Created by mengdongqi on 15-10-24.
//

#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "queue.h"

// create a new adjacency list node
pAdjListNode new_AdjListNode(int index) {
    pAdjListNode newNode = (pAdjListNode) malloc(sizeof(ADJ_LIST_NODE));
    newNode->index = index;
    newNode->next = NULL;
    return newNode;
}

//creates a graph of v_num vertices
pGraph create_graph(USER_INPUT *user_input) {
    pGraph graph = (pGraph) malloc(sizeof(GRAPH));
    convert_input_edges_into_vertices(user_input, graph);
    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (pAdjList) malloc(graph->v_num * sizeof(ADJ_LIST));
    for (int i = 0; i < graph->v_num; i++) {
        graph->array[i].head = NULL;
    }
    populate_edges_into_graph(user_input, graph);
    return graph;
}

// add an edge to an undirected graph ,insert node into front of the adjacency list
void add_edge(pGraph graph, int src_index, int destination_index) {
    // Add an edge from src to destination.
    // A new node(destination) is added to the adjacency list of src.
    // The node(destination) is added at the front of the list of src
    pAdjListNode newNode = new_AdjListNode(destination_index);
    newNode->next = graph->array[src_index].head;
    graph->array[src_index].head = newNode;

    // Since graph is undirected, add an edge from destination to src .
    // A new node(src) is added to the adjacency list of destination.
    // The node(src) is added at the front of the list of destination
    newNode = new_AdjListNode(src_index);
    newNode->next = graph->array[destination_index].head;
    graph->array[destination_index].head = newNode;
}

// print the adjacency list representation of graph
void print_graph(pGraph graph) {
    printf("\nHere is our generated Graph:\n");
    for (int v = 0; v < graph->v_num; ++v) {
        pAdjListNode pCrawl = graph->array[v].head;
        printf("Adjacency list of vertex index %d, vertex value %d\n head ", v, graph->vertices_from_input_edges[v]);
        while (pCrawl != NULL) {
            printf("-> %d", pCrawl->index);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

// first check if vertex exist in this list, then insert int list
void check_and_insert_vertex_into_list(int vertex_value, int list[], int *list_size) {
    for (int i = 0; i < *list_size; i++) {
        if (list[i] == vertex_value)
            return; // found this vertex, just return
    }
    list[*list_size] = vertex_value; // not found this vertex, append this value to the end of this vertex list
    *list_size = *list_size + 1;
}

int index_of(int vertex_value, int list[], int list_size) {
    for (int i = 0; i < list_size; i++) {
        if (list[i] == vertex_value)
            return i;
    }
    return 0;
}

void convert_input_edges_into_vertices(USER_INPUT *user_input, pGraph graph) {
    for (int i = 0; i < user_input->input_edge_num; i++) {
        EDGE e = user_input->input_edges[i];
        check_and_insert_vertex_into_list(e.A, graph->vertices_from_input_edges, &graph->v_num);
        check_and_insert_vertex_into_list(e.B, graph->vertices_from_input_edges, &graph->v_num);
    }
}

void populate_edges_into_graph(USER_INPUT *user_input, pGraph graph) {
    for (int i = 0; i < user_input->input_edge_num; i++) {
        EDGE e = user_input->input_edges[i];
        add_edge(graph,
                 index_of(e.A, graph->vertices_from_input_edges, graph->v_num),
                 index_of(e.B, graph->vertices_from_input_edges, graph->v_num));
    }
}

// get vertex index of vertices generated from input edges
int get_vertex_index_from_input_edges(int value, pGraph graph) {
    for (int i = 0; i < graph->v_num; i++) {
        if (graph->vertices_from_input_edges[i] == value)
            return i;
    }
    return -1;
}

// BFS traverse from start_index to end_index
// if path found return true, else return false
// if path found, stop the traverse and print the path, and return true
bool bfs_traverse(int start_index, int end_index, pGraph graph) {
    bool arrive_end = false;
    for (int i = 0; i < graph->v_num; i++)
        graph->visited[i] = false;
    graph->visited[start_index] = true;
    int iterator_index, deQueue_vertex_index; // iterator index and vertex_index deQueue from the queue
    pQUEUE queue = create_queue();
    en_queue(queue, create_node(start_index, NULL)); // first node has no parent node in BFS tree
    while (queue->queueSize != 0) {
        pNODE node = de_queue(queue);
        deQueue_vertex_index = node->node_id;
        // traverse all adjacency nodes of deQueue_vertex_index
        for (iterator_index = first_vertex(graph, deQueue_vertex_index);
             iterator_index >= 0;
             iterator_index = next_vertex(graph, deQueue_vertex_index, iterator_index)) {
            if (graph->visited[iterator_index] == false) {
                if (DEBUG)
                    printf("visit node: %d\n", iterator_index);
                graph->visited[iterator_index] = true;
                pNODE newNode = create_node(iterator_index, node);
                en_queue(queue, newNode);
                if (newNode->node_id == end_index) {
                    arrive_end = true;
                    print_path(graph, newNode);
                    printf("\n");
                    break;
                }
            }
        }
        if (arrive_end == true) {
            break;
        }

    }
    clean_queue(queue);
    return arrive_end;
}

//get first vertex from an adjacency list start from index
int first_vertex(pGraph graph, int start_index) {
    return graph->array[start_index].head->index;
}

//get next vertex_index from an adjacency list start from index
//return next vertex_index after 'before_index'
int next_vertex(pGraph graph, int start_index, int before_index) {
    pAdjListNode tmpNode = graph->array[start_index].head;
    while (tmpNode->index != before_index) {
        tmpNode = tmpNode->next;
    }
    if (tmpNode->next)
        return tmpNode->next->index;
    else
        return -1;
}

// print path from node
void print_path(pGraph graph, pNODE node) {
    if (node->parentNodeInBFSTree == NULL) {
        printf("%d", graph->vertices_from_input_edges[node->node_id]);
    } else {
        print_path(graph, node->parentNodeInBFSTree);
        printf("-%d", graph->vertices_from_input_edges[node->node_id]);
    }
}

// clear this graph
void clear_graph(pGraph graph) {
    for (int i = 0; i < graph->v_num; i++) {
        pAdjListNode node = graph->array[i].head;
        while (node && node->next) {
            pAdjListNode tmpNode = node;
            node = node->next;
            free(tmpNode);
        }
        free(node);
    }
    free(graph->array);
    free(graph);
}
