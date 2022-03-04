#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"
#include "truthTable.h"

Node * initLinkedList() {
    Node *newNode = malloc(sizeof(Node));
    newNode->data = (size_t) NULL;
    newNode->next = NULL;
    return newNode;
}

void addToLinkedList(Node *head, size_t data) {
    if ((void *) head->data == NULL) {
        head->data = data;
        return;
    }
    Node *newNode = malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = head->next;
    head->next = (struct Node *) newNode;
}

void displayLinkedList(Node *head) {
    Node *current = head;
    if (head == NULL) {
        printf("Linked list is empty. \n");
        return;
    }
    printf("Nodes of linked list: ");
    while (current != NULL) {
        printf("%zu ", current->data);
        current = (Node *) current->next;
    }
    printf("\n");
}

size_t countNodes(Node *head) {
    size_t count = 0;
    Node *current = head;

    while (current != NULL) {
        count += 1;
        current = (Node *) current->next;
    }

    return count;
}

void freeLinkedList(Node *head) {
    Node *current = NULL;
    while (head != NULL) {
        current = head;
        head = (Node *) head->next;
        free(current);
    }
}

/**
 * Initialize a new Linked list of type Truth Table Node
 * @return The new node created
 */
ttNode * initTtNode() {
    ttNode *newNode = malloc(sizeof(ttNode));
    newNode->next = NULL;
    newNode->data = NULL;
    return newNode;
}

/**
 * Add a node to the linked list.
 * @param head The pointer to the head of the linked list
 * @param data The data to add to the list
 */
void addNode(ttNode *head, truthTable *data) {
    // head.data should be the same right? Why is it different?
    if (head->data == NULL) {
        head->data = data;
        return;
    }
    ttNode *newNode = malloc(sizeof(ttNode));
    newNode->data = data;
    newNode->next = head->next;
    head->next = (struct ttNode *) newNode;
}

/**
 * Count the number of nodes in the linked list
 * @param head The pointer to the head of the linked list
 * @return The number of nodes in the linked list
 */
size_t countTtNodes(ttNode *head) {
    if (head == NULL) return 0;
    size_t count = 1;
    ttNode *current = head;
    while (current->next != NULL) {
        count += 1;
        current = (ttNode *) current->next;
    }
    return count;
}

truthTable * getNode(ttNode *head, size_t index) {
    if (index > countTtNodes(head)) {
        printf("The node is out of bounds\n");
        exit(1);
    }
    ttNode *current = head;
    for (int i = 0; i < index; ++i) {
        current = (ttNode *) current->next;
    }
    return current->data;
}

void displayTtLinkedList(ttNode *head) {
    ttNode *current = head;
    if (head == NULL) {
        printf("Linked list is empty. \n");
        return;
    }
    printf("Nodes of linked list: ");
    while (current != NULL) {
        printf("%zu ", (size_t)current);
        current = (ttNode *) current->next;
    }
    printf("\n");
}

void freeTtLinkedList(ttNode *head) {
    ttNode *current = NULL;
    while (head != NULL) {
        current = head;
        head = (ttNode *) head->next;
        destroyTruthTable(current->data);
        free(current);
    }
}
