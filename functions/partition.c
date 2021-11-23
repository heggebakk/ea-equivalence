#include <stdlib.h>
#include <stdbool.h>
#include "partition.h"

/**
 * Partition bucket size where k = even
 * Works only for k = 4
 * @param function The function to be partitioned
 * @param k The size of the tuple T
 * @param target An element of F2^n
 * @return Partitions
 */
partitions partitionFunction(truthTable *function, size_t k, size_t target) {
    if (k % 2 != 0) {
        printf("k is odd, the function partitionFunction works only for even numbers.");
        exit(1);
    }

    size_t *multiplicities = calloc(sizeof(size_t), 1L << function->dimension);

    for (size_t i = 0; i < 1L << function->dimension; ++i) multiplicities[i] = 0;
    // TODO: Make this a recursive function that work for all k = even
    for (size_t x1 = 0; x1 < 1L << function->dimension; ++x1) {
        for (size_t x2 = 0; x2 < 1L << function->dimension; ++x2) {
            for (size_t x3 = 0; x3 < 1L << function->dimension; ++x3) {
                size_t x4 = x1 ^ x2 ^ x3;
                size_t value = function->elements[x1] ^ function->elements[x2] ^ function->elements[x3] ^
                               function->elements[x4];
                multiplicities[value] += 1;
            }
        }
    }

    partitions partition;
    partition.numBuckets = 0;
    partition.multiplicities = calloc(sizeof(size_t), 1L << function->dimension);
    partition.bucketSizes = calloc(sizeof(size_t), 1L << function->dimension);
    partition.buckets = calloc(sizeof(size_t *), 1L << function->dimension);

    for (size_t i = 0; i < 1L << function->dimension; ++i) {
        size_t numBuckets = partition.numBuckets;
        size_t multiplicity = multiplicities[i];
        // Check if multiplicity is in bucket, if false; add multiplicity to bucket
        bool multiplicityInBuckets = false;
        for (size_t b = 0; b < numBuckets; ++b) {
            if (partition.multiplicities[b] == multiplicity) {
                multiplicityInBuckets = true;
                partition.buckets[b][partition.bucketSizes[b]] = i;
                partition.bucketSizes[b] += 1;
                break;
            }
        }
        if (!multiplicityInBuckets) {
            // Add a new bucket to the buckets list
            partition.buckets[numBuckets] = calloc(sizeof(size_t), 1L << function->dimension);
            partition.bucketSizes[numBuckets] = 1;
            partition.multiplicities[numBuckets] = multiplicity;
            partition.buckets[numBuckets][0] = i;
            partition.numBuckets += 1;
        }
    }
    free(multiplicities);

    return partition;
}

void printPartitionInfo(partitions p) {
    for (int i = 0; i < p.numBuckets; ++i) {
        printf("Multiplicity %zu, Size %zu, Buckets: ", p.multiplicities[i], p.bucketSizes[i]);
        for (int j = 0; j < p.bucketSizes[i]; ++j) {
            printf("%zu ", p.buckets[i][j]);
        }
        printf("\n");
    }
}
