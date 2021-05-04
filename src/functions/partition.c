#include <stdlib.h>
#include <stdio.h>
#include "partition.h"
#include "stdbool.h"

typedef struct vbfTruthTable truthTable;
typedef struct vbfPartitions partition;
typedef struct vbfBucket bucket;

void freeBuckets(bucket *buckets);

int compare(const void *a, const void *b) {
    return (*(int *) a - *(int *) b);
}

int comparePartition (const void *a, const void *b) {
    return (int)(((struct vbfBucket*) a) -> size - ((struct vbfBucket*)b) -> size);
}

/*
 * Partition size where k = even
 * Works only for k = 4
 * TODO: Make it work for all k = even
 */
partition partitionFunction(truthTable * function, int k, int t) {
    if (k % 2 != 0) {
        printf("k is not even");
        exit(1);
    }

    size_t *multiplicities;
    multiplicities = malloc(sizeof(size_t) * function->size);
    size_t *uniqueMultiplicities;
    uniqueMultiplicities = malloc(sizeof(size_t) * function->size); // List with the multiplicity
    int uniqueCount = 0;

    for (int i = 0; i < function->size; ++i) {
        multiplicities[i] = 0;
    }

    for (int x1 = 0; x1 < function->size; ++x1) {
        for (int x2 = 0; x2 < function->size; ++x2) {
            for (int x3 = 0; x3 < function->size; ++x3) {
                int x4 = x1 ^x2 ^x3;
                int value = function->elements[x1] ^function->elements[x2] ^function->elements[x3] ^function->elements[x4] ^t;
                multiplicities[value] += 1;
                for (int i = 0; i < uniqueCount; ++i) {
                    if (uniqueMultiplicities[i] != value) {
                        uniqueMultiplicities[uniqueCount] = value;
                        uniqueCount += 1;
                    }
                }
            }
        }
    }
    free(uniqueMultiplicities);
    bucket *buckets;
    buckets = malloc(sizeof(bucket) * function->size);
    int numBuckets = 0;

    for (size_t i = 0; i < function->size; ++i) {
        size_t multiplicity = multiplicities[i];
        // Check if multipl. is in buckets, if false add multip. to buckets
        bool multiplicityInBuckets = false;
        for (int b = 0; b < numBuckets; ++b) {
            if (buckets[b].multiplicity == multiplicity) {
                multiplicityInBuckets = true;
                buckets[b].elements[buckets[b].size - 1] = i;
                buckets[b].size += 1;
                break;
            }
        }
        if (!multiplicityInBuckets) {
            // Add a new bucket to the buckets list
            bucket newBucket;
            newBucket.size = 1;
            newBucket.multiplicity = multiplicity;
            newBucket.elements = malloc((sizeof(size_t) * function->size));
            newBucket.elements[0] = i;
            buckets[numBuckets] = newBucket;
            numBuckets += 1;
            free(newBucket.elements);
        }
    }
    printf("Multiplicities: ");
    for (int i = 0; i < function->size; ++i) {
        printf("%zu ", multiplicities[i]);
    }
    free(multiplicities);

    printf("\n");
    for (int i = 0; i < numBuckets; ++i) {
        printf("%d: ", buckets[i].size);
        for (int j = 0; j < buckets[i].size; ++j) {
            printf("%zu ", buckets[i].elements[j]);
        }
        printf("\n");
    }


    partition partitions;
    partitions.buckets = malloc(sizeof(bucket) * numBuckets);
    partitions.buckets = buckets;
    partitions.dimension = function->dimension;
    partitions.numBuckets = numBuckets;

    qsort(partitions.buckets, partitions.numBuckets, sizeof(struct vbfBucket), comparePartition);

    printf("Partitions: ");
    for (int i = 0; i < partitions.numBuckets; ++i) {
        printf("[%d, %zu] ", partitions.buckets[i].size, partitions.buckets[i].multiplicity);
    }
    printf("\n");

    return partitions;
}

void freeBuckets(bucket *buckets) {
    for (int i = 0; i < buckets->size; ++i) {
        free(buckets[i].elements);
    }
}

void freePartition(partition p) {
    free(p.buckets);
}
