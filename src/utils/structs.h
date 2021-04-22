#ifndef SRC_STRUCTS_H
#define SRC_STRUCTS_H

struct vbfTruthTable {
    size_t dimension;
    size_t elements;
    int * array;
};

struct vbfBucket {
    size_t bucketSize;
    size_t value;
};

struct vbfPartitions {
    size_t dimension;
    size_t numBuckets;
    struct vbfBucket *buckets;
};

struct vbfPermutation {

};

#endif //SRC_STRUCTS_H
