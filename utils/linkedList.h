#ifndef EA_EQUIVALENCE_LINKEDLIST_H
#define EA_EQUIVALENCE_LINKEDLIST_H

#include "stdio.h"
#include "truthTable.h"

/**
 * Represent a node of a linked list
 */
typedef struct {
    size_t data;
    struct Node *next;
} Node ;

Node * initLinkedList();

void addToLinkedList(Node *head, size_t data);

void displayLinkedList(Node *head);

size_t countNodes(Node *head);

void freeLinkedList(Node *head);

typedef struct {
    truthTable *data;
    struct ttNode *next;
} ttNode ;

ttNode * initTtNode();

void addNode(ttNode *head, truthTable *data);

truthTable * getNode(ttNode *head, size_t index);

size_t countTtNodes(ttNode *head);

void displayTtLinkedList(ttNode *head);

void freeTtLinkedList(ttNode *head);

#endif //EA_EQUIVALENCE_LINKEDLIST_H
