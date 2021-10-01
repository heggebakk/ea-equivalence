#ifndef EA_EQUIVALENCE_STRUCTS_H
#define EA_EQUIVALENCE_STRUCTS_H
#include "stdio.h"

typedef struct vbfTruthTable {
    size_t dimension;
    size_t size;
    size_t *elements;
} truthTable;

typedef struct vbfPartitions {
    size_t numBuckets;
    size_t *multiplicities;
    size_t *bucketSizes;
    size_t **buckets;
} partitions;

typedef struct vbfPermutations {
    size_t numPermutations;
    size_t **permutations;
} permutations;

#endif //EA_EQUIVALENCE_STRUCTS_H
