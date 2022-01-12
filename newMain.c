#include <time.h>
#include <stdio.h>
#include <malloc.h>
#include "walshTransform/walshTransform.h"
#include "utils/linkedList.h"
#include "functions/outerPermutation.h"
#include "utils/inverse.h"
#include "functions/innerPermutation.h"
#include "utils/parser.h"
#include "utils/freeMemory.h"
#include "stddef.h"

int runWalshTransform(truthTable *f, truthTable *g, size_t k, size_t dimension, size_t *basis, FILE *fp) {
    fprintf(fp, "\n** WALSH TRANSFORM **\n");
    // Start time
    double totalTime = 0.0;
    clock_t startTotalTime = clock();

    // Change function f and g from a truth table to a walsh transform table
    walshTransform *functionF = truthTableToWalshTransform(*f);
    walshTransform *functionG = truthTableToWalshTransform(*g);

    // Partition function f and g
    double partitionTime = 0.0;
    clock_t startPartitionTime = clock();
    partitions *partitionF = eaPartitionWalsh(functionF, k);
    partitions *partitionG = eaPartitionWalsh(functionG, k);
    clock_t endPartitionTime = clock();
    partitionTime += (double) (endPartitionTime - startPartitionTime) / CLOCKS_PER_SEC;

    // Calculate Outer Permutation
    double outerPermutationTime = 0.0;
    clock_t startOuterPermutationTime = clock();
    struct ttNode *l1 = initNode();
    size_t numPermutations = findOuterPermutation(dimension, partitionF, partitionG, basis, l1, fp);
    clock_t endOuterPermutationTime = clock();
    outerPermutationTime += (double) (endOuterPermutationTime - startOuterPermutationTime) / CLOCKS_PER_SEC;

    // Calculate inner permutation
    double innerPermutationTime = 0.0;
    clock_t startInnerPermutationTime = clock();
    for (size_t i = 0; i < numPermutations; ++i) {
        truthTable *l1Prime = getNode(l1, i);
        truthTable *l1Inverse = inverse(*l1Prime);
        truthTable *gPrime = composeFunctions(l1Inverse, g);
        truthTable *lPrime;
        truthTable *l2 = malloc(sizeof(truthTable));
        l2->dimension = dimension;
        l2->elements = malloc(sizeof(size_t) * 1L << dimension);

        if (innerPermutation(f, gPrime, basis, l2, &lPrime)) {
            clock_t endInnerPermutationTime = clock();
            innerPermutationTime += (double) (endInnerPermutationTime - startInnerPermutationTime) / CLOCKS_PER_SEC;

            // Find l
            truthTable *l = composeFunctions(l1Prime, lPrime);
            writeTruthTable(l1[i].data, fp, "l1");
            writeTruthTable(l2, fp, "l2");
            writeTruthTable(l, fp, "l");

            // Free memory
            freeTruthTable(l);
            freeTruthTable(l1Inverse);
            freeTruthTable(l2);
            freeTruthTable(lPrime);
            freeTruthTable(gPrime);

            break;
        }
        freeTruthTable(l1Inverse);
        freeTruthTable(l2);
        freeTruthTable(gPrime);
    }

    freeTtWt(functionF);
    freeTtWt(functionG);
    freePartition(partitionF);
    freePartition(partitionG);
    freeTtLinkedList(l1);

    // End time
    clock_t endTotalTime = clock();
    totalTime += (double) (endTotalTime - startTotalTime) / CLOCKS_PER_SEC;

    // Print time information
    printf("Walsh Transform:\n");
    printf("Time spent partitioning: %f \n", partitionTime);
    printf("Time spent outer permutation: %f \n", outerPermutationTime);
    printf("Time spent inner permutation: %f \n", innerPermutationTime);
    printf("Total time spent: %f \n", totalTime);


    fprintf(fp, "\nWalsh Transform:\n");
    fprintf(fp, "Time spent partitioning: %f \n", partitionTime);
    fprintf(fp, "Time spent outer permutation: %f \n", outerPermutationTime);
    fprintf(fp, "Time spent inner permutation: %f \n", innerPermutationTime);
    fprintf(fp, "Total time spent: %f \n", totalTime);

    return 0;
}

int newAlgorithm(truthTable *f, truthTable *g, size_t k, size_t dimension, size_t *basis, FILE *fp) {
    fprintf(fp, "\n** NEW ALGORITHM **\n");
    // Start time
    double totalTime = 0.0;
    clock_t startTotalTime = clock();

    // Partition function f and g
    double partitionTime = 0.0;
    clock_t startPartitionTime = clock();
    partitions *partitionF = partitionFunction(f, k);
    partitions *partitionG = partitionFunction(g, k);
    // TODO: Check if partition f and g is compatible
    clock_t endPartitionTime = clock();
    partitionTime += (double) (endPartitionTime - startPartitionTime) / CLOCKS_PER_SEC;

    // Outer permutation
    double outerPermutationTime = 0.0;
    clock_t startOuterPermutation = clock();
    struct ttNode *l1 = initNode();
    size_t numPermutations = findOuterPermutation(dimension, partitionF, partitionG, basis, l1, fp);
    clock_t endOuterPermutation = clock();
    outerPermutationTime += (double) (endOuterPermutation - startOuterPermutation) / CLOCKS_PER_SEC;

    // Inner permutation
    double innerPermutationTime = 0.0;
    clock_t startInnerPermutation = clock();
    for (size_t i = 0; i < numPermutations; ++i) {
        truthTable *l1Prime = getNode(l1, i);
        truthTable *l1Inverse = inverse(*l1Prime);
        truthTable *gPrime = composeFunctions(l1Inverse, g);
        truthTable *lPrime;
        truthTable *l2 = malloc(sizeof(truthTable));
        l2->dimension = dimension;
        l2->elements = malloc(sizeof(size_t) * 1L << dimension);

        if (innerPermutation(f, gPrime, basis, l2, &lPrime)) {
            clock_t endInnerPermutation = clock();
            innerPermutationTime += (double) (endInnerPermutation - startInnerPermutation) / CLOCKS_PER_SEC;

            // Find l
            truthTable *l = composeFunctions(l1Prime, lPrime);

            writeTruthTable(l1[i].data, fp, "l1");
            writeTruthTable(l2, fp, "l2");
            writeTruthTable(l, fp, "l");

            freeTruthTable(l);
            freeTruthTable(l1Inverse);
            freeTruthTable(l2);
            freeTruthTable(lPrime);
            freeTruthTable(gPrime);
            break;
        }
        freeTruthTable(l1Inverse);
        freeTruthTable(l2);
        freeTruthTable(gPrime);
    }

    freeTruthTable(f);
    freeTruthTable(g);
    freePartition(partitionF);
    freePartition(partitionG);
    freeTtLinkedList(l1);

    // End time
    clock_t endTotalTime = clock();
    totalTime += (double) (endTotalTime - startTotalTime) / CLOCKS_PER_SEC;

    printf("New algorithm: \n");
    printf("Time spent partitioning: %f \n", partitionTime);
    printf("Time spent outer permutation: %f \n", outerPermutationTime);
    printf("Time spent inner permutation: %f \n", innerPermutationTime);
    printf("Total time spent: %f \n", totalTime);

    fprintf(fp, "\nNew algorithm:\n");
    fprintf(fp, "Time spent partitioning: %f \n", partitionTime);
    fprintf(fp, "Time spent outer permutation: %f \n", outerPermutationTime);
    fprintf(fp, "Time spent inner permutation: %f \n", innerPermutationTime);
    fprintf(fp, "Total time spent: %f \n", totalTime);

    return 0;
}


