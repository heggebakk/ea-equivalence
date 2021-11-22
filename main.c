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
        printf("Expected 2 arguments");
        return 1;
    }

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
    partitions partitionF = partitionFunction(functionF, k, target);
    partitions partitionG = partitionFunction(functionG, k, target);
    // TODO: Check if partition f and g is compatible
    clock_t endPartition = clock();
    timeSpentPartition += (double) (endPartition - startPartition) / CLOCKS_PER_SEC;

    printf("Results from partitioning function F: \n");
    printPartitionInfo(partitionF);
    printf("Results from partitioning function G: \n");
    printPartitionInfo(partitionG);
    printf("\n");

    size_t *basis = createBasis(DIMENSION); // Standard basis
    double timeSpentPermutation = 0.0;
    clock_t startPermutation = clock();

    struct ttNode * l1 = initNode();
    outerPermutation(partitionF, partitionG, DIMENSION, basis, l1);
    printf("\n l1 data: \n");
    printTruthTable(*l1->data);
    size_t numPerm = countTtNodes(l1);
    clock_t endPermutation = clock();
    timeSpentPermutation += (double) (endPermutation - startPermutation) / CLOCKS_PER_SEC;
    printf("Number of permutations: %zu \n", numPerm);

    for (size_t i = 0; i < numPerm; ++i) {
        truthTable l1Prime = getNode(l1, i);
        printf("l1 prime: \n");
        printTruthTable(l1Prime);
        truthTable l1Inverse = inverse(l1Prime);
        printf("l1 inverse: \n");
        printTruthTable(l1Inverse);
        truthTable * gPrime = composeFunctions(&l1Inverse, &functionG);
        truthTable * lPrime;

        truthTable innerPerm;
        innerPerm.dimension = DIMENSION;
        innerPerm.elements = calloc(sizeof(size_t), 1L << DIMENSION);
        if (innerPermutation(&functionF, gPrime, basis, &innerPerm, &lPrime)) {
            printf("SUCCESS!\n\n");
            // have lprime and l2, compute l1,l2,l (l = l1 comp. lprime)
            truthTable * l = composeFunctions(&l1Prime, lPrime);
            printTruthTable(*l);

            truthTable * test = composeFunctions(&functionF, &innerPerm);
            truthTable *oldtest = test; //for freedom purposes
            test = composeFunctions(&l1Prime, test);
            addFunctionsTogether(test, &functionG);
            printf("Test function is linear: %s\n", isLinear(test) ? "true" : "false");
            freeTruthTable(test);
            freeTruthTable(oldtest);

            // Check if l' is correct:
            truthTable * x = composeFunctions(&functionF, &innerPerm);
            addFunctionsTogether(x, gPrime);
            printf("g' is = to l' %s\n", compareTruthTable(*x, *lPrime) ? "true" : "false");
            freeTruthTable(x);

            // This is a test! :)
            truthTable * t = composeFunctions(&functionF, &innerPerm);
            truthTable *t2 = t; //freedom!!!
            t = composeFunctions(&l1Prime, t);
            addFunctionsTogether(t, l);
            printf("Functions is equal: %s\n",compareTruthTable(*t, functionG) ? "true" : "false");
            printf("\n");
            freeTruthTable(t);
            freeTruthTable(t2);

            freeTruthTable(l);
            free(l1Inverse.elements);
            free(innerPerm.elements);
            freeTruthTable(lPrime);
            freeTruthTable(gPrime);
            break;
        }
        free(l1Inverse.elements);
        free(innerPerm.elements);
//        freeTruthTable(lPrime);
        freeTruthTable(gPrime);
    }

    freeTruthTable(functionF);
    freeTruthTable(functionG);
    free(basis);
    freePartition(partitionF);
    freePartition(partitionG);
    freeTtLinkedList(l1);

    printf("Time spent parsing: %f seconds \n", timeSpentParsing);
    printf("Time spent partitioning: %f seconds \n", timeSpentPartition);
    printf("Time spent outer permutation: %f seconds\n", timeSpentPermutation);

    return 0;
}