#include <malloc.h>
#include "linkedList.h"

typedef struct ttNode ttNode;

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

void addNode(struct ttNode *head, truthTable *data) {
    if (head->data == NULL) {
        head->data = data;
        return;
    }
    struct ttNode *newNode = malloc(sizeof(ttNode));
    newNode->data = data;
    newNode->next = head->next;
    head->next = newNode;
}

struct ttNode initNode() {
    struct ttNode newNode;
    newNode.next = NULL;
    newNode.data = NULL;
    return newNode;
}

