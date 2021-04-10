#include <stdlib.h>
#include <stdio.h>
#include "partition.h"

typedef struct vbfTruthTable truthTable;
typedef struct vbfPartitions partition;

int compare(const void *a, const void *b) {
    return (*(int *) a - *(int *) b);
}

int comparePartition (const void *a, const void *b) {
    return (int)(((struct vbfPartition*) a) -> bucketSize - ((struct vbfPartition*)b) -> bucketSize);
}

/*
 * Works only for k = 4
 * TODO: Make it work for all k = even
 */
void partitionElements(truthTable * tt, int k, int t) {
    if (k % 2 != 0) {
        printf("k is not even");
        exit(1);
    }

    size_t *multiplicities;
    multiplicities = malloc(sizeof(size_t) * tt->elements);

    for (int i = 0; i < tt->elements; ++i) {
        multiplicities[i] = 0;
    }

    for (int x1 = 0; x1 < tt->elements; ++x1) {
        for (int x2 = 0; x2 < tt->elements; ++x2) {
            for (int x3 = 0; x3 < tt->elements; ++x3) {
                int x4 = x1 ^ x2 ^ x3;
                int value = tt->array[x1] ^ tt->array[x2] ^ tt->array[x3] ^ tt->array[x4] ^ t;
                multiplicities[value] += 1;
            }
        }
    }

    qsort(multiplicities, tt->elements, sizeof(size_t), compare);

//    for (int i = 0; i < tt->elements; i++) {
//        printf("%zu, ", multiplicities[i]);
//    }
//    printf("\n");

    size_t current = multiplicities[0];
    int numBuckets = 0;
    int count = 0;
    for (int i = 0; i < tt->elements; ++i) {
        if (multiplicities[i] != current) {
            current = multiplicities[i];
            count = 0;
            numBuckets += 1;
        }
        count += 1;
    }
    numBuckets += 1;
    printf("Num buckets: %d\n", numBuckets);

    partition partitions;
    partitions.buckets = malloc(sizeof (struct vbfPartition) * numBuckets);
    partitions.dimension = tt->dimension;

    current = multiplicities[0];
    partitions.numBuckets = 0;
    count = 0;
    for (int i = 0; i < tt->elements; ++i) {
        if (multiplicities[i] != current) {
            partitions.buckets[partitions.numBuckets].bucketSize = count;
            partitions.buckets[partitions.numBuckets].value = current;
            current = multiplicities[i];
            count = 0;
            partitions.numBuckets += 1;
        }
        count += 1;
    }

    partitions.buckets[partitions.numBuckets].bucketSize = count;
    partitions.buckets[partitions.numBuckets].value = current;
    partitions.numBuckets += 1;

    qsort(partitions.buckets, partitions.numBuckets, sizeof(struct vbfPartition), comparePartition);

    printf("Partitions: ");
    for (int i = 0; i < partitions.numBuckets; ++i) {
        printf("[%zu, %zu] ", partitions.buckets[i].bucketSize, partitions.buckets[i].value);
    }
}
