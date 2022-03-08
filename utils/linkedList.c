#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"
#include "truthTable.h"

Node *initLinkedList() {
    Node *newNode = malloc(sizeof(Node));
    newNode->data = -1;
    newNode->next = NULL;
    return newNode;
}

void addToLinkedList(Node *head, size_t data) {
    if (head->data == -1) {
        head->data = data;
        return;
    }
    Node *newNode = malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = head->next;
    head->next = (struct Node*) newNode;
}

void printLinkedList(Node *head) {
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

void destroyLinkedList(Node *head) {
    Node *current = NULL;
    while (head != NULL) {
        current = head;
        head = (Node *) head->next;
        free(current);
    }
}

TtNode * initTtNode() {
    TtNode *newNode = malloc(sizeof(TtNode));
    newNode->next = NULL;
    newNode->data = NULL;
    return newNode;
}

void addNode(TtNode *head, TruthTable *data) {
    if (head->data == NULL) {
        head->data = data;
        return;
    }
    TtNode *newNode = malloc(sizeof(TtNode));
    newNode->data = data;
    newNode->next = head->next;
    head->next = (struct ttNode *) newNode;
}

size_t countTtNodes(TtNode *head) {
    if (head == NULL) return 0;
    size_t count = 1;
    TtNode *current = head;
    while (current->next != NULL) {
        count += 1;
        current = (TtNode *) current->next;
    }
    return count;
}

TruthTable * getNode(TtNode *head, size_t index) {
    if (index > countTtNodes(head)) {
        printf("The node is out of bounds\n");
        exit(1);
    }
    TtNode *current = head;
    for (int i = 0; i < index; ++i) {
        current = (TtNode *) current->next;
    }
    return current->data;
}

void printTtLinkedList(TtNode *head) {
    TtNode *current = head;
    if (head == NULL) {
        printf("Linked list is empty. \n");
        return;
    }
    printf("Nodes of linked list: ");
    while (current != NULL) {
        for (int i = 0; i < 1L << current->data->dimension; ++i) {
            if (i == (1L << current->data->dimension) - 1) {
                printf("%zu\n", current->data->elements[i]);
            } else {
                printf("%zu ", current->data->elements[i]);
            }
        }
        current = (TtNode *) current->next;
    }
    printf("\n");
}

void writeTtLinkedList(TtNode *head, FILE *fp) {
    TtNode *current = head;
    while (current != NULL) {
        for (int i = 0; i < 1L << current->data->dimension; ++i) {
            if (i == (1L << current->data->dimension) - 1) {
                fprintf(fp, "%zu\n", current->data->elements[i]);
            } else {
                fprintf(fp, "%zu ", current->data->elements[i]);
            }
        }
        current = (TtNode *) current->next;
    }
}

void destroyTtLinkedList(TtNode *head) {
    TtNode *current = NULL;
    while (head != NULL) {
        current = head;
        head = (TtNode *) head->next;
        destroyTruthTable(current->data);
        free(current);
    }
}
