#include <stdlib.h>
#include "freeMemory.h"

void freeTruthTable(truthTable tt) {
    free(tt.elements);
}

void freePartition(partitions p) {
    for (size_t i = 0; i < p.numBuckets; ++i) {
        free(p.buckets[i]);
    }
    free(p.bucketSizes);
    free(p.multiplicities);
    free(p.buckets);
}

void freePermutations(permutations p) {
    for (size_t i = 0; i < p.numPermutations; ++i) {
        free(p.permutations[i]);
    }
    free(p.permutations);
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
        freeTruthTable(*current->data);
        free(current);
    }
}
