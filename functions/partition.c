#include <stdlib.h>
#include <stdbool.h>
#include "partition.h"

/**
 * Partition bucket size where k = even
 * Works only for k = 4
 * @param function The function to be partitioned
 * @param k The size of the tuple T
 * @param t An element of F2^n
 * @return Partitions
 */
partitions partitionFunction(truthTable *function, size_t k, size_t t) {
    if (k % 2 != 0) {
        printf("k is odd, the function partitionFunction works only for even numbers.");
        exit(1);
    }

    size_t *multiplicities = malloc(sizeof(size_t) * function->size);
    size_t *uniqueMultiplicities = malloc(sizeof(size_t) * function->size); // List of multiplicities
    size_t uniqueCount = 0;

    for (size_t i = 0; i < function->size; ++i) multiplicities[i] = 0;
    // TODO: Make this a recursive function that work for all k = even
    for (size_t x1 = 0; x1 < function->size; ++x1) {
        for (size_t x2 = 0; x2 < function->size; ++x2) {
            for (size_t x3 = 0; x3 < function->size; ++x3) {
                size_t x4 = x1 ^ x2 ^ x3;
                size_t value = function->elements[x1] ^ function->elements[x2] ^ function->elements[x3] ^
                               function->elements[x4];
                multiplicities[value] += 1;
                for (size_t i = 0; i < uniqueCount; ++i) {
                    if (uniqueMultiplicities[i] != value) {
                        uniqueMultiplicities[uniqueCount] = value;
                        uniqueCount += 1;
                    }
                }
            }
        }
    }

    free(uniqueMultiplicities);
    bucket **buckets = malloc(sizeof(bucket *) * function->size);
    int numBuckets = 0;

    for (size_t i = 0; i < function->size; ++i) {
        size_t multiplicity = multiplicities[i];
        // Chack if plultiplicity is in bucket, if false; add multiplicity to bucket
        bool multiplicityInBuckets = false;
        for (size_t b = 0; b < numBuckets; ++b) {
            if (buckets[b]->multiplicity == multiplicity) {
                multiplicityInBuckets = true;
                buckets[b]->elements[buckets[b]->bucketSize] = i;
                buckets[b]->bucketSize += 1;
                break;
            }
        }
        if (!multiplicityInBuckets) {
            // Add a new bucket to the buckets list
            buckets[numBuckets] = malloc(sizeof(bucket));
            buckets[numBuckets]->bucketSize = 1;
            buckets[numBuckets]->multiplicity = multiplicity;
            buckets[numBuckets]->elements = malloc(sizeof(size_t) * function->size);
            buckets[numBuckets]->elements[0] = i;
            numBuckets += 1;
        }
    }
    free(multiplicities);

    partitions p;
    p.buckets = buckets;
    p.dimension = function->dimension;
    p.numBuckets = numBuckets;

    return p;
}

void printPartitionInfo(partitions p) {
    for (int i = 0; i < p.numBuckets; ++i) {
        printf("%zu: ", p.buckets[i]->bucketSize);
        for (int j = 0; j < p.buckets[i]->bucketSize; ++j) {
            printf("%zu ", p.buckets[i]->elements[j]);
        }
        printf("\n");
    }
}
