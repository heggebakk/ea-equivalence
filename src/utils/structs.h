#ifndef SRC_STRUCTS_H
#define SRC_STRUCTS_H
#include "stdio.h"

struct vbfTruthTable {
    int dimension;
    size_t elements;
    int *array;
};

/**
 * A bucket contains a list of all elements and the size of the bucket.
 */
struct vbfBucket {
    size_t multiplicity;
    size_t *elements;
    int size;
};

struct vbfPartitions {
    int dimension;
    size_t numBuckets;
    struct vbfBucket *buckets;
};

struct vbfLinearPermutation{

};

#endif //SRC_STRUCTS_H
