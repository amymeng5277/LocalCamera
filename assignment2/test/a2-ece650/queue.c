//
// Created by mengdongqi on 15-10-24.
//

#include <stdlib.h>
#include <stdio.h>
#include "queue.h"


pQUEUE create_queue() {
    pQUEUE queue = (pQUEUE) malloc(sizeof(QUEUE));
    queue->queueSize = 0;
    queue->headNode = NULL;
    queue->tailNode = NULL;
    queue->node_list = NULL;
    return queue;
}

// add node after tail of queue
void en_queue(pQUEUE queue, pNODE node) {
    if (queue->headNode == NULL) {
        queue->headNode = node;
    }
    queue->queueSize++;
    if (queue->tailNode != NULL) {
        queue->tailNode->nextNodeInQueue = node;
    }
    queue->tailNode = node;
    if (DEBUG)
        printf("insert node: %d into queue, queue size now: %d\n", node->node_id, queue->queueSize);
    queue->node_list = realloc(queue->node_list, queue->queueSize * sizeof(pNODE));
    queue->node_list[queue->queueSize - 1] = node;
}

// get first node of this queue
pNODE de_queue(pQUEUE queue) {
    pNODE node = queue->headNode;
    queue->headNode = queue->headNode->nextNodeInQueue;
    queue->queueSize--;
    if (DEBUG)
        printf("take node: %d from queue, queue size now: %d\n", node->node_id, queue->queueSize);
    return node;
}

// clear this queue, free all space
void clean_queue(pQUEUE queue) {
    for (int i = 0; i < queue->queueSize; i++) {
        free(queue->node_list[i]);
    }
    free(queue);
}