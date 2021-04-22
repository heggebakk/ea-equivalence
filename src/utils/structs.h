#ifndef SRC_STRUCTS_H
#define SRC_STRUCTS_H

struct vbfTruthTable {
    size_t dimension;
    size_t elements;
    int * array;
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
    size_t dimension;
    size_t numBuckets;
    struct vbfBucket *buckets;
};

#endif //SRC_STRUCTS_H
