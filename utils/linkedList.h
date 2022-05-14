#ifndef EA_EQUIVALENCE_LINKEDLIST_H
#define EA_EQUIVALENCE_LINKEDLIST_H

#include "stdio.h"
#include "../structures.h"

/**
 * Represent a node of a linked list
 */
typedef struct Node {
    size_t data;
    struct Node *next;
} Node ;

/**
 * Initialize a new linked list of struct Node
 * @return A new empty linked list
 */
Node * initLinkedList();

/**
 * Add a new node to a linked list.
 * @param head The head of the linked list
 * @param data The data for the node to be added
 */
void addToLinkedList(Node *head, size_t data);

/**
 * Print a linked list to the console
 * @param head The head of the linked list
 */
void printLinkedList(Node *head);

/**
 * Counts all the nodes in a linked list and returns the result
 * @param head The head of the linked list
 * @return The number of nodes in the linked list
 */
size_t countNodes(Node *head);

/**
 * Destroy a linked list of struct Node
 * @param head The head of the Linked List
 */
void destroyLinkedList(Node *head);

typedef struct TtNode {
    TruthTable *data;
    struct TtNode *next;
} TtNode ;

/**
 * Initialize a new Linked list of type Truth Table Node
 * @return The new node created
 */
TtNode * initTtNode();

/**
 * Add a node to the linked list.
 * @param head The pointer to the head of the linked list
 * @param data The data to add to the list
 */
void addNode(TtNode *head, TruthTable *data);

/**
 * Count the number of nodes in the linked list
 * @param head The pointer to the head of the linked list
 * @return The number of nodes in the linked list
 */
TruthTable * getNode(TtNode *head, size_t index);

/**
 * Counts all the nodes in a linked list and returns the result
 * @param head The head of the linked list
 * @return The number of nodes in the linked list
 */
size_t countTtNodes(TtNode *head);

/**
 * Print linked list to the console
 * @param head The head of the linked list
 */
void printTtLinkedList(TtNode *head);

/**
 * Write all truth tables from a linked list to a file
 * @param head The head of the linked list
 * @param fp The file to write to
 */
void writeTtLinkedList(TtNode *head, FILE *fp);

/**
 * Destroy Linked List of struct TtNode
 * @param head The head of the linked list
 */
void destroyTtLinkedList(TtNode *head);

#endif //EA_EQUIVALENCE_LINKEDLIST_H
