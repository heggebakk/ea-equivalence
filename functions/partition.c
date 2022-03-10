#include <stdlib.h>
#include <string.h>
#include "partition.h"

MappingOfClasses *initMappingsOfClasses() {
    MappingOfClasses *newMap = malloc(sizeof(MappingOfClasses));
    newMap->numOfMappings = 0;
    return newMap;
}

void addToMOC(MappingOfClasses *moc, size_t *mapping, size_t *domain, size_t mappingSize, size_t domainSize) {
    moc->mappings[moc->numOfMappings] = malloc(sizeof(size_t *) * mappingSize);
    moc->domains[moc->numOfMappings] = malloc(sizeof (size_t *) * domainSize);
    memcpy(moc->mappings[moc->numOfMappings], mapping, mappingSize);
    memcpy(moc->domains[moc->numOfMappings], domain, domainSize);
    moc->numOfMappings += 1;
}

void destroyMappingOfClasses(MappingOfClasses *mappingsOfClasses) {
    for (int i = 0; i < mappingsOfClasses->numOfMappings; ++i) {
        free(mappingsOfClasses->domains[i]);
        free(mappingsOfClasses->mappings[i]);
    }
    free(mappingsOfClasses->domains);
    free(mappingsOfClasses->mappings);
    free(mappingsOfClasses);
}

Partition *initPartition(size_t size) {
    Partition *newPartition = malloc(sizeof(Partition));
    newPartition->numberOfClasses = 0;
    newPartition->multiplicities = malloc(sizeof(size_t) * size);
    newPartition->classSizes = malloc(sizeof(size_t) * size);
    newPartition->classes = malloc(sizeof(size_t *) * size);
}

Partition *partitionFunction(TruthTable *function, size_t k) {
    if (k % 2 != 0) {
        printf("k is odd, the function partitionFunction works only for even numbers.");
        exit(1);
    }

    size_t *multiplicities = calloc(sizeof(size_t), 1L << function->dimension);

    for (size_t i = 0; i < 1L << function->dimension; ++i) multiplicities[i] = 0;
    findAllMultiplicities(k, 0, multiplicities, function, 0, 0);

    Partition *partition = initPartition(1L << function->dimension);

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

void findAllMultiplicities(size_t k, int i, size_t *multiplicities, TruthTable *function, size_t x, size_t value) {
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

void printPartitionInfo(Partition *partition) {
    printf("Partition:\n%zu\n", partition->numberOfClasses);
    for (int i = 0; i < partition->numberOfClasses; ++i) {
        for (int j = 0; j < partition->classSizes[i]; ++j) {
            if (j == partition->classSizes[i] - 1) {
                printf("%zu\n", partition->classes[i][j]);
            } else {
                printf("%zu ", partition->classes[i][j]);
            }
        }
    }
    printf("\n");
}

void writePartition(Partition *partition, FILE *fp) {
    fprintf(fp, "%zu\n", partition->numberOfClasses);
    for (int i = 0; i < partition->numberOfClasses; ++i) {
        for (int j = 0; j < partition->classSizes[i]; ++j) {
            if (j == partition->classSizes[i] - 1) {
                fprintf(fp, "%zu\n", partition->classes[i][j]);
            } else {
                fprintf(fp, "%zu ", partition->classes[i][j]);
            }
        }
    }
}

void **mapPartitionClasses(Partition *partitionF, Partition *partitionG, size_t dimension, MappingOfClasses *mappingOfClasses) {
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
    Node **domains = malloc(sizeof(Node) * partitionF->numberOfClasses);
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
        printLinkedList(domains[i]);
    }

    // Find out how many mappings there is
    size_t numOfMappings = 1; // There is always at least one mapping
    bool *isCalculated = malloc(sizeof(bool) * partitionF->numberOfClasses);
    for (int i = 0; i < partitionF->numberOfClasses; ++i) isCalculated[i] = false;
    for (int i = 0; i < partitionF->numberOfClasses; ++i) {
        Node *current = (Node *) domains[i]->next;
        if (!isCalculated[current->data]) {
            numOfMappings = numOfMappings * factorial(countNodes(domains[i]));
            isCalculated[current->data] = true;
        }
    }
    free(isCalculated);

    // Create a list of different domain mappings
    mappingOfClasses->domains = malloc(sizeof(size_t *) * numOfMappings);

    // Recursive part.
    mappingOfClasses->mappings = malloc(sizeof(size_t *) * numOfMappings);
    createMappings(mappingOfClasses, domains, partitionG, dimension, numOfMappings);

    // Free domains
    for (int i = 0; i < partitionF->numberOfClasses; ++i) {
        destroyLinkedList(domains[i]);
    }
    free(domains);
}

void
createMappings(MappingOfClasses *mappingOfClasses, Node **domains, Partition *partitionG, size_t dimension, size_t numOfMappings) {
    for (size_t i = 0; i < numOfMappings; ++i) {
        size_t *newList = malloc(sizeof(size_t) * 1L << dimension);
        bool *chosen = malloc(sizeof(bool) * partitionG->numberOfClasses); // A boolean map with the size of number of classes
        size_t *currentDomain = malloc(sizeof(size_t) * partitionG->numberOfClasses);
        for (int j = 0; j < partitionG->numberOfClasses; ++j) {
            chosen[j] = false;
        }
        selectRecursive(0, newList, currentDomain, chosen, domains, partitionG, dimension, mappingOfClasses);
        free(chosen);
    }
}

void
selectRecursive(size_t i, size_t *newList, size_t *currentDomain, bool *chosen, Node **domains, Partition *partitionG,
                size_t dimension, MappingOfClasses *moc) {
    if (i >= partitionG->numberOfClasses) {
        printf("newlist: ");
        for (int j = 0; j < 1L << dimension; ++j) {
            printf("%zu ", newList[j]);
        }
        printf("\n");
        // Add the new list and the matching domain map to the mappingOfClasses.
        addToMOC(moc, newList, currentDomain, 1L << dimension, partitionG->numberOfClasses);
        return;
    }
    Node *current = (Node *) domains[i]->next; // Tells us which bucket we are going through, starting with the first possible matching
    while (current != NULL) {
        if (!chosen[current->data]) {
            currentDomain[i] = current->data;
            for (int j = 0; j < partitionG->classSizes[current->data]; ++j) {
                newList[partitionG->classes[current->data][j]] = current->data;
            }
            chosen[current->data] = true;
            selectRecursive(i + 1, newList, currentDomain, chosen, domains, partitionG, dimension, moc);
        }
        if (current->next == NULL) {
            chosen[current->data] = false;
        }
        current = (Node *) current->next;
    }
}

size_t factorial(size_t value) {
    size_t fact = 1;
    for (int i = 1; i < value + 1; ++i) {
        fact *= i;
    }
    return fact;
}

void destroyPartitions(Partition *p) {
    for (size_t i = 0; i < p->numberOfClasses; ++i) {
        free(p->classes[i]);
    }
    free(p->classSizes);
    free(p->multiplicities);
    free(p->classes);
    free(p);
}
