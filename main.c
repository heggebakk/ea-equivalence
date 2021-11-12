#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "time.h"
#include "utils/structs.h"
#include "utils/parser.h"
#include "functions/partition.h"
#include "utils/freeMemory.h"
#include "functions/outerPermutation.h"
#include "functions/innerPermutation.h"
#include "utils/inverse.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Expected 2 arguments");
        return 1;
    }
    char *truthTable1 = argv[1];
    char *truthTable2 = argv[2];

    double timeSpentParsing = 0.0;
    clock_t startParsing = clock();
    truthTable functionF = parseTruthTable(truthTable1);
    truthTable functionG = parseTruthTable(truthTable2);
    clock_t endParsing = clock();
    timeSpentParsing += (double) (endParsing - startParsing) / CLOCKS_PER_SEC;
    printTruthTable(functionF);
    printTruthTable(functionG);
    printf("\n");
    size_t DIMENSION = functionF.dimension;

    size_t k = 4;
    size_t target = 0;
    double timeSpentPartition = 0.0;
    clock_t startPartition = clock();
    partitions partitionF = partitionFunction(&functionF, k, target);
    partitions partitionG = partitionFunction(&functionG, k, target);
    // TODO: Check if partition f and g is compatible

    clock_t endPartition = clock();
    timeSpentPartition += (double) (endPartition - startPartition) / CLOCKS_PER_SEC;

    printf("Results from partition function F: \n");
    printPartitionInfo(partitionF);
    printf("Results from partition function G: \n");
    printPartitionInfo(partitionG);
    printf("\n");

    size_t *basis = createBasis(DIMENSION); // Standard basis
    double timeSpentPermutation = 0.0;
    clock_t startPermutation = clock();

    size_t NUMBER_OF_TRUTH_TABLES = 1L << DIMENSION << DIMENSION;
    truthTable **outerPerm = calloc(sizeof (truthTable *), NUMBER_OF_TRUTH_TABLES);
    for(size_t i = 0 ; i < NUMBER_OF_TRUTH_TABLES; ++i) {
        outerPerm[i] = malloc(sizeof(truthTable));
        outerPerm[i]->elements = calloc(sizeof(size_t), 1L << DIMENSION);
        outerPerm[i]->dimension = DIMENSION;
    }
    size_t numPerm = outerPermutation(partitionF, partitionG, DIMENSION, basis, outerPerm);
    clock_t endPermutation = clock();
    timeSpentPermutation += (double) (endPermutation - startPermutation) / CLOCKS_PER_SEC;
    printf("Number of permutations: %zu \n", numPerm);

    for (size_t i = 0; i < numPerm; ++i) {
        truthTable innerPerm;
        innerPerm.dimension = DIMENSION;
        innerPerm.elements = calloc(sizeof(size_t), 1L << DIMENSION);
        truthTable l1Inverse = inverse(*outerPerm[i]);
        truthTable gPrime = composeFunctions(&l1Inverse, &functionG);

        if (innerPermutation(&functionF, &gPrime, basis, &innerPerm, NULL)) {
            printf("SUCCESS!\n");
            // have lprime and l2, compute l1,l2,l (l = l1 comp. lprime)
            break;
        }
        freeTruthTable(l1Inverse);
        freeTruthTable(innerPerm);
    }

    freeTruthTable(functionF);
    freeTruthTable(functionG);
    free(basis);
    for (size_t i = 0; i < numPerm; ++i) {
        freeTruthTable(*outerPerm[i]);
    }
    freePartition(partitionF);
    freePartition(partitionG);

    printf("Time spent parsing: %f seconds \n", timeSpentParsing);
    printf("Time spent partitioning: %f seconds \n", timeSpentPartition);
    printf("Time spent outer permutation: %f seconds\n", timeSpentPermutation);

    return 0;
}