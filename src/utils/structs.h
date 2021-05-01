#ifndef SRC_STRUCTS_H
#define SRC_STRUCTS_H
#include "stdio.h"

struct vbfTruthTable {
    size_t dimension;
    size_t size;
    int * elements;
};

void freeTruthTable(struct vbfTruthTable tt);

/**
 * A bucket contains a list of all size and the size of the bucket.
 */
struct vbfBucket {
    size_t multiplicity;
    size_t *elements;
    int size;
};

struct vbfPartitions {
    size_t dimension;
    size_t numBuckets;
    struct vbfBucket *buckets;
};

void freePartition(struct vbfPartitions p);

void freeBuckets(struct vbfBucket *buckets);

struct vbfPermutation {

};

#endif //SRC_STRUCTS_H
