#ifndef EA_EQUIVALENCE_STRUCTS_H
#define EA_EQUIVALENCE_STRUCTS_H
#include "stdio.h"

typedef struct vbfTruthTable {
    size_t dimension;
    size_t size;
    size_t *elements;
} truthTable;

typedef struct vbfBucket {
    size_t multiplicity;
    size_t *elements;
    size_t bucketSize;
} bucket;

typedef struct vbfPartitions {
    size_t dimension;
    size_t numBuckets; // TODO: Rename numBuckets to something that gives more meaning
    bucket **buckets;
} partitions;

typedef struct vbfPermutations {
    size_t numPermutations;
    size_t *permutations;
} permutations;

#endif //EA_EQUIVALENCE_STRUCTS_H
