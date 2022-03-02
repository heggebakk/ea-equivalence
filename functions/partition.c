#include <stdlib.h>
#include <string.h>
#include "partition.h"

MappingsOfClasses *initMappingsOfClasses() {
    MappingsOfClasses *newMap = malloc(sizeof(MappingsOfClasses));
    newMap->mappings = malloc(sizeof(size_t *));
    newMap->domains = malloc(sizeof(size_t *));
    newMap->numOfMappings = 0;
}

void destroyMappingOfClasses(MappingsOfClasses *mappingsOfClasses) {
    for (int i = 0; i < mappingsOfClasses->numOfMappings; ++i) {
        free(mappingsOfClasses->domains[i]);
        free(mappingsOfClasses->mappings[i]);
    }
    free(mappingsOfClasses->domains);
    free(mappingsOfClasses->mappings);
    free(mappingsOfClasses);
}

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

void **mapPartitionClasses(partitions *partitionF, partitions *partitionG, size_t dimension, MappingsOfClasses *mappingOfClasses) {
    // Check if the Partition has the same number of classes. If the sizes of the classes is different,
    // we have an invalid solution.
    if (partitionF->numberOfClasses != partitionG->numberOfClasses) {
        printf("The partition of function F and G is not compatible!\n");
        printf("Partition F has %zu number of classes and partition G has %zu number of classes.\n",
               partitionF->numberOfClasses, partitionG->numberOfClasses);
        exit(1);
    }

    // Find all the domains for the different classes.
    // Example where we have several mappings:
    // [(0,2),(1),(0,2),(3,4),(3,4),(5)]
    struct Node **domains = malloc(sizeof(struct Node) * partitionF->numberOfClasses);
    for (int i = 0; i < partitionF->numberOfClasses; ++i) {
        domains[i] = initLinkedList();
    }
    for (int i = 0; i < partitionF->numberOfClasses; ++i) {
        bool sameSizes = false;
        for (int j = 0; j < partitionG->numberOfClasses; ++j) {
            if (partitionF->classSizes[i] == partitionG->classSizes[j]) {
                addToLinkedList(domains[i], j);
                sameSizes = true;
            }
        }
        if (!sameSizes) {
            printf("Partition F and G does not have the same class sizes!");
            exit(1);
        }
    }

    // Find out how many mappings there is
    mappingOfClasses->numOfMappings = 1; // There is always at least one mapping
    for (int i = 0; i < partitionF->numberOfClasses; ++i) {
        mappingOfClasses->numOfMappings *= factorial(countNodes(domains[i]));
    }

    // Create a list of different domain mappings
    mappingOfClasses->domains = malloc(sizeof(size_t) * mappingOfClasses->numOfMappings);

    // Recursive part.
    mappingOfClasses->mappings = malloc(sizeof(size_t) * mappingOfClasses->numOfMappings);
    createMappings(mappingOfClasses, domains, partitionG, dimension);
    // Free domains
    for (int i = 0; i < partitionF->numberOfClasses; ++i) {
        freeLinkedList(domains[i]);
    }
    free(domains);
}

void
createMappings(MappingsOfClasses *mappingOfClasses, struct Node **domains, partitions *partitionG, size_t dimension) {
    for (size_t i = 0; i < mappingOfClasses->numOfMappings; ++i) {
        size_t *newList = malloc(sizeof(size_t) * 1L << dimension);
        bool *chosen = malloc(sizeof(bool) * partitionG->numberOfClasses); // A boolean map with the size of number of classes
        size_t *currentDomain = malloc(sizeof(size_t) * partitionG->numberOfClasses);
        for (int j = 0; j < partitionG->numberOfClasses; ++j) {
            chosen[j] = false;
        }
        selectRecursive(0, newList, currentDomain, chosen, domains, partitionG, dimension);
        mappingOfClasses->mappings[i] = newList;
        mappingOfClasses->domains[i] = currentDomain;
        free(chosen);
    }
}

void selectRecursive(size_t i, size_t *newList, size_t *currentDomain, bool *chosen, struct Node **domains,
                     partitions *partitionG, size_t dimension) {
    if (i >= partitionG->numberOfClasses) {
        return;
    }
    struct Node *current = domains[i]; // Tells us which bucket we are going through, starting with the first possible matching
    while (current != NULL) {
        if (chosen[current->data] == false) {
            currentDomain[i] = current->data;
            for (int j = 0; j < partitionG->classSizes[current->data]; ++j) {
                newList[partitionG->classes[current->data][j]] = current->data;
                chosen[current->data] = true;
                selectRecursive(i + 1, newList, currentDomain, chosen, domains, partitionG, dimension);
            }
        }
        current = current->next;
    }
}

size_t factorial(size_t value) {
    size_t fact = 1;
    for (int i = 1; i < value; ++i) {
        fact *= i;
    }
    return fact;
}

void destroyPartitions(partitions *p) {
    for (size_t i = 0; i < p->numberOfClasses; ++i) {
        free(p->classes[i]);
    }
    free(p->classSizes);
    free(p->multiplicities);
    free(p->classes);
    free(p);
}