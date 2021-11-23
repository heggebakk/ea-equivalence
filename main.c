#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "utils/structs.h"
#include "utils/parser.h"
#include "functions/partition.h"
#include "utils/freeMemory.h"
#include "functions/outerPermutation.h"
#include "functions/innerPermutation.h"
#include "utils/inverse.h"
#include "utils/compareTruthTable.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Expected 2 truth table files");
        return 1;
    }

    double totalRunTime = 0.0;
    clock_t startRunTime = clock();

    // Parse files to truth tables
    double timeSpentParsing = 0.0;
    clock_t startParsing = clock();
    truthTable *functionF = parseTruthTable(argv[1]);
    truthTable *functionG = parseTruthTable(argv[2]);
    clock_t endParsing = clock();
    timeSpentParsing += (double) (endParsing - startParsing) / CLOCKS_PER_SEC;

    printTruthTable(functionF);
    printTruthTable(functionG);
    printf("\n");
    size_t DIMENSION = functionF->dimension;

    // TODO: Take in k from input variable
    size_t k = 4;
    size_t target = 0;
    // Partition function F and G
    double timeSpentPartition = 0.0;
    clock_t startPartition = clock();
    partitions *partitionF = partitionFunction(functionF, k, target);
    partitions *partitionG = partitionFunction(functionG, k, target);
    // TODO: Check if partition f and g is compatible
    clock_t endPartition = clock();
    timeSpentPartition += (double) (endPartition - startPartition) / CLOCKS_PER_SEC;

    printf("Results from partitioning function F: \n");
    printPartitionInfo(partitionF);
    printf("Results from partitioning function G: \n");
    printPartitionInfo(partitionG);
    printf("\n");

    size_t *basis = createBasis(DIMENSION); // Standard basis

    // Outer Permutation, L_1
    double timeSpentOuterPermutation = 0.0;
    clock_t startPermutation = clock();
    struct ttNode * l1 = initNode();
    outerPermutation(partitionF, partitionG, DIMENSION, basis, l1);
    size_t numPerm = countTtNodes(l1);
    clock_t endPermutation = clock();
    timeSpentOuterPermutation += (double) (endPermutation - startPermutation) / CLOCKS_PER_SEC;

    printf("Number of permutations: %zu \n", numPerm);

    double timeSpentInnerPermutation = 0.0;
    for (size_t i = 0; i < numPerm; ++i) {
        truthTable *l1Prime = getNode(l1, i);
        truthTable l1Inverse = inverse(*l1Prime);
        truthTable * gPrime = composeFunctions(&l1Inverse, functionG);
        truthTable * lPrime;

        clock_t startInnerPermutation = clock();
        truthTable *l2 = malloc(sizeof(truthTable));
        l2->dimension = DIMENSION;
        l2->elements = calloc(sizeof(size_t), 1L << DIMENSION);
        if (innerPermutation(functionF, gPrime, basis, l2, &lPrime)) {
            clock_t endInnerPermutation = clock();
            timeSpentInnerPermutation += (double) (endInnerPermutation - startInnerPermutation) / CLOCKS_PER_SEC;
//            printf("SUCCESS!\n\n");
            // have l' and l2, compute l1,l2,l (l = l1 composed with l')
            truthTable *l = composeFunctions(l1Prime, lPrime);

            // Check if l' is correct:
            truthTable *x = composeFunctions(functionF, l2);
            addFunctionsTogether(x, gPrime);
            if (!compareTruthTable(*x, *lPrime)) {
                printf("l' is not correct! Something is wrong!\n");
            }
            freeTruthTable(x);

            printf("l1: \n");
            printTruthTable(l1[i].data);
            printf("l2: \n");
            printTruthTable(l2);
            printf("l: \n");
            printTruthTable(l);
            printf("\n");

            freeTruthTable(l);
            free(l1Inverse.elements);
            freeTruthTable(l2);
            freeTruthTable(lPrime);
            freeTruthTable(gPrime);
            break;
        }
        free(l1Inverse.elements);
        freeTruthTable(l2);
        freeTruthTable(gPrime);
    }

    freeTruthTable(functionF);
    freeTruthTable(functionG);
    free(basis);
    freePartition(partitionF);
    freePartition(partitionG);
    freeTtLinkedList(l1);

    clock_t stopTotalRunTime = clock();
    totalRunTime += (double) (stopTotalRunTime - startRunTime) / CLOCKS_PER_SEC;

    printf("Time spent parsing: %f seconds \n", timeSpentParsing);
    printf("Time spent partitioning: %f seconds \n", timeSpentPartition);
    printf("Time spent outer permutation: %f seconds \n", timeSpentOuterPermutation);
    printf("Time spent inner permutation: %f seconds \n", timeSpentInnerPermutation);
    printf("Total time spent: %f seconds \n", totalRunTime);

    return 0;
}