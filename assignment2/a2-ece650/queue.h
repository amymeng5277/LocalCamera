//
// Created by mengdongqi on 15-10-24.
//
#include "base.h"

#ifndef ASSIGNMENT2_QUEUE_H
#define ASSIGNMENT2_QUEUE_H

typedef struct Queue QUEUE;
typedef struct Queue *pQUEUE;

struct Queue {
    pNODE headNode;
    pNODE tailNode;
    pNODE *node_list; // to store all nodes refs, for destroy this queue
    int queueSize;
};

pQUEUE create_queue();

// add node after tail of queue
void en_queue(pQUEUE queue, pNODE node);

// get first node of this queue
pNODE de_queue(pQUEUE queue);

// clear this queue, free all space
void clean_queue(pQUEUE queue);

#endif //ASSIGNMENT2_QUEUE_H
