#include <stdlib.h>
#include <stdio.h>
#include "partition.h"

typedef struct vbfTruthTable truthTable;
typedef struct vbfPartitions partition;
typedef struct vbfBucket bucket;

int compare(const void *a, const void *b) {
    return (*(int *) a - *(int *) b);
}

int comparePartition (const void *a, const void *b) {
    return (int)(((struct vbfBucket*) a) -> size - ((struct vbfBucket*)b) -> size);
}

/*
 * Partition elements where k = even
 * Works only for k = 4
 * TODO: Make it work for all k = even
 */
partition partitionFunction(truthTable * function, int k, int t) {
    if (k % 2 != 0) {
        printf("k is not even");
        exit(1);
    }

    size_t *multiplicities;
    multiplicities = malloc(sizeof(size_t) * function->elements);

    for (int i = 0; i < function->elements; ++i) {
        multiplicities[i] = 0;
    }

    for (int x1 = 0; x1 < function->elements; ++x1) {
        for (int x2 = 0; x2 < function->elements; ++x2) {
            for (int x3 = 0; x3 < function->elements; ++x3) {
                int x4 = x1 ^x2 ^x3;
                int value = function->array[x1] ^function->array[x2] ^function->array[x3] ^function->array[x4] ^t;
                multiplicities[value] += 1;
            }
        }
    }

    qsort(multiplicities, function->elements, sizeof(size_t), compare);

    size_t current = multiplicities[0];
    int numBuckets = 0;
    int count = 0;
    // Count the number of different multiplicities
    for (int i = 0; i < function->elements; ++i) {
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
    partitions.buckets = malloc(sizeof (struct vbfBucket) * numBuckets);
    partitions.dimension = function->dimension;

    current = multiplicities[0];
    partitions.numBuckets = 0;
    count = 0;
    for (int i = 0; i < function->elements; ++i) {
        if (multiplicities[i] != current) {
            partitions.buckets[partitions.numBuckets].size= count;
            partitions.buckets[partitions.numBuckets].elements = current;
            current = multiplicities[i];
            count = 0;
            partitions.numBuckets += 1;
        }
        count += 1;
    }

    partitions.buckets[partitions.numBuckets].size = count;
    partitions.buckets[partitions.numBuckets].elements = current;
    partitions.numBuckets += 1;

    qsort(partitions.buckets, partitions.numBuckets, sizeof(struct vbfBucket), comparePartition);

    printf("Partitions: ");
    for (int i = 0; i < partitions.numBuckets; ++i) {
        printf("[%zu, %zu] ", partitions.buckets[i].size, partitions.buckets[i].elements);
    }
    printf("\n");
    return partitions;
}
