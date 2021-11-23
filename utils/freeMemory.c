#include <stdlib.h>
#include "freeMemory.h"
#include "parser.h"

void freeTruthTable(truthTable *tt) {
    free(tt->elements);
    free(tt);
}

void freePartition(partitions *p) {
    for (size_t i = 0; i < p->numBuckets; ++i) {
        free(p->buckets[i]);
    }
    free(p->bucketSizes);
    free(p->multiplicities);
    free(p->buckets);
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
