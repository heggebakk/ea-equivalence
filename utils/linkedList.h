#ifndef EA_EQUIVALENCE_LINKEDLIST_H
#define EA_EQUIVALENCE_LINKEDLIST_H

#include "stdio.h"

/**
 * Represent a node of a linked list
 */
struct Node {
    size_t data;
    struct Node *next;
};

void displayLinkedList(struct Node *head);

size_t countNodes(struct Node *head);

#endif //EA_EQUIVALENCE_LINKEDLIST_H
