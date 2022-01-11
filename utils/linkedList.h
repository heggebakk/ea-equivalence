#ifndef EA_EQUIVALENCE_LINKEDLIST_H
#define EA_EQUIVALENCE_LINKEDLIST_H

#include "stdio.h"
#include "truthTable.h"

/**
 * Represent a node of a linked list
 */
struct Node {
    size_t data;
    struct Node *next;
};

void displayLinkedList(struct Node *head);

size_t countNodes(struct Node *head);

struct ttNode {
    truthTable *data;
    struct ttNode *next;
};

struct ttNode * initNode();

void addNode(struct ttNode *head, truthTable *data);

truthTable * getNode(struct ttNode *head, size_t index);

size_t countTtNodes(struct ttNode *head);

void displayTtLinkedList(struct ttNode *head);

#endif //EA_EQUIVALENCE_LINKEDLIST_H
