#ifndef EA_EQUIVALENCE_STRUCTS_H
#define EA_EQUIVALENCE_STRUCTS_H
#include "stdio.h"

typedef struct vbfTruthTable {
    size_t dimension;
    size_t *elements;
} truthTable;

typedef struct vbfPartitions {
    size_t numBuckets;
    size_t *multiplicities;
    size_t *bucketSizes;
    size_t **buckets;
} partitions;

#endif //EA_EQUIVALENCE_STRUCTS_H
