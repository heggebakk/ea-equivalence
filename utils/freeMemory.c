#include <stdlib.h>
#include "freeMemory.h"
#include "parser.h"

void freeTruthTable(truthTable *tt) {
    free(tt->elements);
    free(tt);
}

void freePartition(partitions *p) {
    for (size_t i = 0; i < p->numberOfClasses; ++i) {
        free(p->classes[i]);
    }
    free(p->classSizes);
    free(p->multiplicities);
    free(p->classes);
    free(p);
}

void freeLinkedList(struct Node *head) {
    struct Node *current = NULL;
    while (head != NULL) {
        current = head;
        head = head->next;
        free(current);
    }
}

void freeTtLinkedList(struct ttNode *head) {
    struct ttNode *current = NULL;
    while (head != NULL) {
        current = head;
        head = head->next;
        freeTruthTable(current->data);
        free(current);
    }
}
