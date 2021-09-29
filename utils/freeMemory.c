#include <stdlib.h>
#include "freeMemory.h"

void freeTruthTable(truthTable tt) {
    free(tt.elements);
}

void freeBuckets(partitions *p) {
    for (size_t i = 0; i < p->numBuckets; ++i) {
        free(p->buckets[i]->elements);
    }
}

void freePartition(partitions p) {
    for (size_t i = 0; i < p.numBuckets; ++i) {
        free(p.buckets[i]);
    }
    free(p.buckets);
}

void freePermutations(permutations *p) {
    for (size_t i = 0; i < p->numPermutations; ++i) {
        free(p[i].permutations);
    }
    free(p);
}
