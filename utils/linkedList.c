#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"
#include "truthTable.h"

Node *initLinkedList() {
    Node *newNode = malloc(sizeof(Node));
    newNode->data = 0;
    newNode->next = NULL;
    return newNode;
}

void addToLinkedList(Node *head, size_t data) {
    Node *newNode = initLinkedList();
    newNode->data = data;
    newNode->next = head->next;
    head->next = newNode;
}

void printLinkedList(Node *head) {
    if (head->next == NULL) {
        printf("Linked list is empty. \n");
        return;
    }
    Node *current = head->next;
    printf("Nodes of linked list: ");
    while (current != NULL) {
        printf("%zu ", current->data);
        current = current->next;
    }
    printf("\n");
}

size_t countNodes(Node *head) {
    size_t count = 0;
    Node *current = head->next;

    while (current != NULL) {
        count += 1;
        current = current->next;
    }

    return count;
}

void destroyLinkedList(Node *head) {
    Node *current = NULL;
    while (head != NULL) {
        current = head;
        head = head->next;
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
        head->data = initTruthTable(data->dimension);
        memcpy(head->data->elements, data->elements, sizeof(size_t) * 1L << data->dimension);
        return;
    }
    TtNode *newNode = malloc(sizeof(TtNode));
    newNode->data = initTruthTable(data->dimension);
    memcpy(newNode->data->elements, data->elements, sizeof(size_t) * 1L << data->dimension);
    newNode->next = head->next;
    head->next = newNode;
}

size_t countTtNodes(TtNode *head) {
    if (head->data == NULL) return 0;
    size_t count = 1;
    TtNode *current = head;
    while (current->next != NULL) {
        count += 1;
        current = current->next;
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
        current = current->next;
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
        current = current->next;
    }
}

void destroyTtLinkedList(TtNode *head) {
    if (head->data == NULL) {
        free(head);
        return;
    }
    while (head->data != NULL) {
        TtNode *current = head;
        head = head->next;
        destroyTruthTable(current->data);
        free(current);
        if (head == NULL) {
            free(head);
            break;
        }
    }
}
