#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "partition.h"
#include "../utils/dataStructures.h"

partitions *initPartition(size_t size) {
    partitions *newPartition = malloc(sizeof(partitions));
    newPartition->numberOfClasses = 0;
    newPartition->multiplicities = malloc(sizeof(size_t) * size);
    newPartition->classSizes = malloc(sizeof(size_t) * size);
    newPartition->classes = malloc(sizeof(size_t *) * size);
}

partitions *partitionFunction(truthTable *function, size_t k) {
    if (k % 2 != 0) {
        printf("k is odd, the function partitionFunction works only for even numbers.");
        exit(1);
    }

    size_t *multiplicities = calloc(sizeof(size_t), 1L << function->dimension);

    for (size_t i = 0; i < 1L << function->dimension; ++i) multiplicities[i] = 0;
    findAllMultiplicities(k, 0, multiplicities, function, 0, 0);

    partitions *partition = initPartition(1L << function->dimension);

    for (size_t i = 0; i < 1L << function->dimension; ++i) {
        size_t numBuckets = partition->numberOfClasses;
        size_t multiplicity = multiplicities[i];
        // Check if multiplicity is in bucket, if false; add multiplicity to bucket
        bool multiplicityInBuckets = false;
        for (size_t b = 0; b < numBuckets; ++b) {
            if (partition->multiplicities[b] == multiplicity) {
                multiplicityInBuckets = true;
                partition->classes[b][partition->classSizes[b]] = i;
                partition->classSizes[b] += 1;
                break;
            }
        }
        if (!multiplicityInBuckets) {
            // Add a new bucket to the classes list
            partition->classes[numBuckets] = calloc(sizeof(size_t), 1L << function->dimension);
            partition->classSizes[numBuckets] = 1;
            partition->multiplicities[numBuckets] = multiplicity;
            partition->classes[numBuckets][0] = i;
            partition->numberOfClasses += 1;
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
    for (int i = 0; i < p->numberOfClasses; ++i) {
        printf("%zu, %zu, ", p->multiplicities[i], p->classSizes[i]);
        for (int j = 0; j < p->classSizes[i]; ++j) {
            printf("%zu ", p->classes[i][j]);
        }
        printf("\n");
    }
}

void mapPartitionClasses(partitions *partitionF, partitions *partitionG) {
    printPartitionInfo(partitionF);
    printPartitionInfo(partitionG);
    printf("\n");

    // Check if the Partition has the same number of classes. If the sizes of the classes is different,
    // we have an invalid solution.
    if (partitionF->numberOfClasses != partitionG->numberOfClasses) {
        printf("The partition of function F and G is not compatible!\n");
        printf("Partition F has %zu number of classes and partition G has %zu number of classes.\n",
               partitionF->numberOfClasses, partitionG->numberOfClasses);
        exit(1);
    }
    // Check if the Partition has the same sizes in the partition classes
    size_t *sortedClassesF = malloc(sizeof(size_t) * partitionF->numberOfClasses);
    size_t *sortedClassesG = malloc(sizeof(size_t) * partitionG->numberOfClasses);
    memcpy(sortedClassesF, partitionF->classSizes, sizeof(size_t) * partitionF->numberOfClasses);
    memcpy(sortedClassesG, partitionG->classSizes, sizeof(size_t) * partitionG->numberOfClasses);
    mergeSort(sortedClassesF, (size_t) 0, partitionF->numberOfClasses - 1);
    mergeSort(sortedClassesG, (size_t) 0, partitionG->numberOfClasses - 1);
    printf("\n");
    for (size_t i = 0; i < partitionF->numberOfClasses; ++i) {
        // Check if the sizes of the classes is the same for both Partition
        if (sortedClassesF[i] != sortedClassesG[i]) {
            printf("Partition F and G does not have the same class sizes!");
            exit(1);
        }
    }
    free(sortedClassesF);
    free(sortedClassesG);


    Set *fSet = initSet(partitionF->numberOfClasses);
    Set *gSet = initSet(partitionG->numberOfClasses);
    for (int i = 0; i < partitionF->numberOfClasses; ++i) {
        insertSet(fSet, partitionF->classSizes[i]);
        insertSet(gSet, partitionG->classSizes[i]);
    }

    size_t numOfMappings = partitionF->numberOfClasses - fSet->length + 1;
    HashMap *hashMap = malloc(sizeof(HashMap *) * numOfMappings);
    hashMap[0] = *initHashMap(partitionF->numberOfClasses);

    // Create the first mapping of the Partition for F and G
    for (size_t i = 0; i < partitionF->numberOfClasses; ++i) {
        for (size_t j = 0; j < partitionG->numberOfClasses; ++j) {
            if (partitionF->classSizes[i] == partitionG->classSizes[j]) {
                insertHashMap(&hashMap[0], i, j);
                break;
            }
        }
    }
    printHashMap(&hashMap[0]);

    // Recursively find all mappings that is valid
    for (int i = 1; i < numOfMappings; ++i) {
        hashMap[i] = *initHashMap(partitionF->numberOfClasses);
        for (int j = 0; j < partitionF->numberOfClasses; ++j) {
            for (int k = 0; k < partitionG->numberOfClasses; ++k) {
                if (partitionF->classSizes[j] == partitionG->classSizes[k]) {
                    insertHashMap(&hashMap[i], j, k);
                }
            }
        }
    }

    destroySet(fSet);
    destroySet(gSet);
}

void merge(size_t *arr, size_t lo, size_t mid, size_t hi) {
    size_t n1 = mid - lo + 1;
    size_t n2 = hi - mid;

    size_t left[n1], right[n2];

    for (size_t i = 0; i < n1; ++i) left[i] = arr[lo + i];
    for (size_t i = 0; i < n2; i++) right[i] = arr[mid + 1 + i];
    size_t i = 0;
    size_t j = 0;
    size_t k = lo;

    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = left[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void mergeSort(size_t *arr, size_t lo, size_t hi) {
    if (lo < hi) {
        size_t m = lo + (hi - lo) / 2;

        mergeSort(arr, lo, m);
        mergeSort(arr, m + 1, hi);

        merge(arr, lo, m, hi);
    }
}
