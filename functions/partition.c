#include <stdlib.h>
#include <stdbool.h>
#include "partition.h"

partitions *partitionFunction(truthTable *function, size_t k) {
    if (k % 2 != 0) {
        printf("k is odd, the function partitionFunction works only for even numbers.");
        exit(1);
    }

    size_t *multiplicities = calloc(sizeof(size_t), 1L << function->dimension);

    for (size_t i = 0; i < 1L << function->dimension; ++i) multiplicities[i] = 0;
    findAllMultiplicities(k, 0, multiplicities, function, 0, 0);

    partitions *partition = malloc(sizeof(partitions));
    partition->numBuckets = 0;
    partition->multiplicities = calloc(sizeof(size_t), 1L << function->dimension);
    partition->bucketSizes = calloc(sizeof(size_t), 1L << function->dimension);
    partition->buckets = calloc(sizeof(size_t *), 1L << function->dimension);

    for (size_t i = 0; i < 1L << function->dimension; ++i) {
        size_t numBuckets = partition->numBuckets;
        size_t multiplicity = multiplicities[i];
        // Check if multiplicity is in bucket, if false; add multiplicity to bucket
        bool multiplicityInBuckets = false;
        for (size_t b = 0; b < numBuckets; ++b) {
            if (partition->multiplicities[b] == multiplicity) {
                multiplicityInBuckets = true;
                partition->buckets[b][partition->bucketSizes[b]] = i;
                partition->bucketSizes[b] += 1;
                break;
            }
        }
        if (!multiplicityInBuckets) {
            // Add a new bucket to the buckets list
            partition->buckets[numBuckets] = calloc(sizeof(size_t), 1L << function->dimension);
            partition->bucketSizes[numBuckets] = 1;
            partition->multiplicities[numBuckets] = multiplicity;
            partition->buckets[numBuckets][0] = i;
            partition->numBuckets += 1;
        }
    }
    free(multiplicities);

    return partition;
}

void findAllMultiplicities(size_t k, int i, size_t *multiplicities, truthTable *function, size_t x, size_t value) {
    if (i == k - 1) {
        size_t newValue = value ^ function->elements[x];
        multiplicities[newValue] += 1;
        return;
    }
    for (int y = 0; y < 1L << function->dimension; ++y) {
        size_t newX = x ^ y;
        size_t newValue = value ^ function->elements[y];
        findAllMultiplicities(k, i + 1, multiplicities, function, newX, newValue);
    }
}

void printPartitionInfo(partitions *p) {
    for (int i = 0; i < p->numBuckets; ++i) {
        printf("%zu, %zu, ", p->multiplicities[i], p->bucketSizes[i]);
        for (int j = 0; j < p->bucketSizes[i]; ++j) {
            printf("%zu ", p->buckets[i][j]);
        }
        printf("\n");
    }
}
