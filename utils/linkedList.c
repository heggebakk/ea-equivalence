#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"
#include "truthTable.h"

typedef struct ttNode ttNode;

struct Node *initLinkedList() {
    struct Node *newNode = (struct Node*) malloc(sizeof(struct Node));
    newNode->next = NULL;
    newNode->data = (size_t) NULL;
    return newNode;
}

void addToLinkedList(struct Node *head, size_t data) {
    if ((void *) head->data == NULL) {
        head->data = data;
        return;
    }
    struct Node *newNode = (struct Node*) malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = head->next;
    head->next = newNode;
}


void displayLinkedList(struct Node *head) {
    struct Node *current = head;
    if (head == NULL) {
        printf("Linked list is empty. \n");
        return;
    }
    printf("Nodes of linked list: ");
    while (current != NULL) {
        printf("%zu ", current->data);
        current = current->next;
    }
    printf("\n");
}

size_t countNodes(struct Node *head) {
    size_t count = 0;
    struct Node *current = head;

    while (current != NULL) {
        count += 1;
        current = current->next;
    }

    return count;
}

void freeLinkedList(struct Node *head) {
    struct Node *current = NULL;
    while (head != NULL) {
        current = head;
        head = head->next;
        free(current);
    }
}

/**
 * Initialize a new Linked list of type Truth Table Node
 * @return The new node created
 */
struct ttNode * initNode() {
    struct ttNode *newNode = (struct ttNode*) malloc(sizeof(ttNode));
    newNode->next = NULL;
    newNode->data = NULL;
    return newNode;
}

/**
 * Add a node to the linked list.
 * @param head The pointer to the head of the linked list
 * @param data The data to add to the list
 */
void addNode(struct ttNode *head, truthTable *data) {
    // head.data should be the same right? Why is it different?
    if (head->data == NULL) {
        head->data = data;
        return;
    }
    struct ttNode *newNode = (struct ttNode*) malloc(sizeof(ttNode));
    newNode->data = data;
    newNode->next = head->next;
    head->next = newNode;
}

/**
 * Count the number of nodes in the linked list
 * @param head The pointer to the head of the linked list
 * @return The number of nodes in the linked list
 */
size_t countTtNodes(struct ttNode *head) {
    if (head == NULL) return 0;
    size_t count = 1;
    struct ttNode *current = head;
    while (current->next != NULL) {
        count += 1;
        current = current->next;
    }
    return count;
}

truthTable * getNode(struct ttNode *head, size_t index) {
    if (index > countTtNodes(head)) {
        printf("The node is out of bounds\n");
        exit(1);
    }
    struct ttNode *current = head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

void displayTtLinkedList(struct ttNode *head) {
    struct ttNode *current = head;
    if (head == NULL) {
        printf("Linked list is empty. \n");
        return;
    }
    printf("Nodes of linked list: ");
    while (current != NULL) {
        printf("%zu ", (size_t)current);
        current = current->next;
    }
    printf("\n");
}

void freeTtLinkedList(struct ttNode *head) {
    struct ttNode *current = NULL;
    while (head != NULL) {
        current = head;
        head = head->next;
        destroyTruthTable(current->data);
        free(current);
    }
}
