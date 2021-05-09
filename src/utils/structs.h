#ifndef SRC_STRUCTS_H
#define SRC_STRUCTS_H
#include "stdio.h"

typedef struct vbfTruthTable {
    size_t dimension;
    size_t size;
    int * elements;
} truthTable;

/**
 * A bucket contains information about the multiplicity, a list of all elements in the bucket and the total size of the
 * bucket.
 */
typedef struct vbfBucket {
    size_t multiplicity;
    size_t *elements;
    int bucketSize;
} bucket;

typedef struct vbfPartitions {
    size_t dimension;
    size_t numBuckets;
    struct vbfBucket ** buckets;
} partitions;

typedef struct array {
    int size;
    size_t *array;
} array;

void freeTruthTable(truthTable tt);

void freePartition(partitions p);

void freeBuckets(partitions *);

#endif //SRC_STRUCTS_H
