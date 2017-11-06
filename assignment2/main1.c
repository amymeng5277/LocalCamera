//
// Created by mengdongqi on 15-10-24.
//

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "base.h"

int main1() {

    pQUEUE queue = create_queue();
    pNODE rootNode = create_node(0, NULL);
    en_queue(queue, rootNode);
    pNODE node1Level_node1 = create_node(1, rootNode);
    en_queue(queue, node1Level_node1);
    pNODE node1Level_node2 = create_node(2, rootNode);
    en_queue(queue, node1Level_node2);
    pNODE node1Level_node3 = create_node(3, rootNode);
    en_queue(queue, node1Level_node3);
    pNODE node1Level_node4 = create_node(4, rootNode);
    en_queue(queue, node1Level_node4);

    de_queue(queue);
    de_queue(queue);
    de_queue(queue);
    de_queue(queue);
    de_queue(queue);

    return 0;
}

