#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "time.h"
#include "utils/truthTable.h"
#include "utils/parser.h"
#include "functions/outerPermutation.h"
#include "utils/eaTest.h"
#include "walshTransform/walshTransform.h"
#include "utils/inverse.h"
#include "functions/innerPermutation.h"

int runOriginal(truthTable *f, truthTable *g, size_t k, size_t dimension, size_t *basis, FILE *fp);

int runWalshTransform(truthTable *f, truthTable *g, size_t k, size_t dimension, size_t *basis, FILE *fp);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Expected at least 2 arguments!");
        return 1;
    }

    long k = 4;
    if (argv[2] != NULL) {
        char* p;
        errno = 0;
        k = strtol(argv[2], &p, 10);
        if (*p != '\0' || errno != 0) {
            printf("Conversion went wrong!\n");
            return 1;
        }
        printf("k = %ld\n", k);
    }
    // Parse files to truth tables
    double timeSpentParsing = 0.0;
    clock_t startParsing = clock();
    truthTable *functionF = parseTruthTable(argv[1]);
    truthTable *functionG = getFunctionG(functionF);
    clock_t endParsing = clock();
    timeSpentParsing += (double) (endParsing - startParsing) / CLOCKS_PER_SEC;

    printf("Time spent parsing files: %f\n", timeSpentParsing);
    printf("\n");

    char *filename = "result.txt";
    FILE *fp = fopen(filename, "w+");
    fprintf(fp, "%s\n", argv[1]);

    printTruthTable(functionF);
    printTruthTable(functionG);
    fprintf(fp, "// Dimension:\n%zu\n", functionF->dimension);
    writeTruthTable(functionF, fp, "F");
    writeTruthTable(functionG, fp, "G");
    size_t DIMENSION = functionF->dimension;
    size_t *basis = createBasis(DIMENSION); // Standard basis

    // Solve with Walsh transform first:
    runWalshTransform(functionF, functionG, k, DIMENSION, basis, fp);
    printf("\n");

    // Solve with new algorithm:
    runOriginal(functionF, functionG, k, DIMENSION, basis, fp);

    fclose(fp);
    free(basis);

    return 0;
}

int runWalshTransform(truthTable *f, truthTable *g, size_t k, size_t dimension, size_t *basis, FILE *fp) {
    fprintf(fp, "\n** WALSH TRANSFORM **\n");
    printf("Walsh transform\n");
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

    /* Walsh Transform specific part done */

    // We might end up in a situation where we have more than one mapping of the partitions from F and G.
    // In this case, we must try and fail. If we succeed, we can finish, otherwise we need to try again.
    MappingOfClasses *mappingOfClassesF = initMappingsOfClasses();
    MappingOfClasses *mappingOfClassesG = initMappingsOfClasses();
    mapPartitionClasses(partitionG, partitionF, dimension, mappingOfClassesF);
    mapPartitionClasses(partitionF, partitionG, dimension, mappingOfClassesG);

    // Calculate Outer Permutation
    double outerPermutationTime = 0.0;
    clock_t startOuterPermutationTime = clock();
    ttNode *l1 = initTtNode();
    size_t numPermutations = outerPermutation(partitionF, partitionG, dimension, basis, l1,
                                              mappingOfClassesG->mappings[0], mappingOfClassesG->domains[0], fp,
                                              mappingOfClassesF->mappings[0]);
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
            destroyTruthTable(l);
            destroyTruthTable(l1Inverse);
            destroyTruthTable(l2);
            destroyTruthTable(lPrime);
            destroyTruthTable(gPrime);

            break;
        }
        destroyTruthTable(l1Inverse);
        destroyTruthTable(l2);
        destroyTruthTable(gPrime);
    }

    destroyWalshTransform(functionF);
    destroyWalshTransform(functionG);
    destroyPartitions(partitionF);
    destroyPartitions(partitionG);
    destroyMappingOfClasses(mappingOfClassesF);
    destroyMappingOfClasses(mappingOfClassesG);
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

int runOriginal(truthTable *f, truthTable *g, size_t k, size_t dimension, size_t *basis, FILE *fp) {
    fprintf(fp, "\n** NEW ALGORITHM **\n");
    printf("New algorithm\n");
    // Start time
    double totalTime = 0.0;
    clock_t startTotalTime = clock();

    // Partition function f and g
    double partitionTime = 0.0;
    clock_t startPartitionTime = clock();
    partitions *partitionF = partitionFunction(f, k);
    partitions *partitionG = partitionFunction(g, k);

    // We might end up in a situation where we have more than one mapping of the partitions from F and G.
    // In this case, we must try and fail. If we succeed, we can finish, otherwise we need to try again.
    MappingOfClasses *mappingOfClassesF = initMappingsOfClasses();
    MappingOfClasses *mappingOfClassesG = initMappingsOfClasses();
    mapPartitionClasses(partitionG, partitionF, dimension, mappingOfClassesF);
    mapPartitionClasses(partitionF, partitionG, dimension, mappingOfClassesG);

    clock_t endPartitionTime = clock();
    partitionTime += (double) (endPartitionTime - startPartitionTime) / CLOCKS_PER_SEC;

    // We need a for loop to go through all the possible mappings. If we succeed, we're finished and breaks out.

    // Outer permutation
    double outerPermutationTime = 0.0;
    clock_t startOuterPermutation = clock();
    ttNode *l1 = initTtNode();

    size_t numPermutations = outerPermutation(partitionF, partitionG, dimension, basis, l1,
                                              mappingOfClassesG->mappings[0], mappingOfClassesG->domains[0], fp,
                                              mappingOfClassesF->mappings[0]); // TODO: Fix this to be a for loop
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

            destroyTruthTable(l);
            destroyTruthTable(l1Inverse);
            destroyTruthTable(l2);
            destroyTruthTable(lPrime);
            destroyTruthTable(gPrime);
            break;
        }
        destroyTruthTable(l1Inverse);
        destroyTruthTable(l2);
        destroyTruthTable(gPrime);
    }

    destroyTruthTable(f);
    destroyTruthTable(g);
    destroyPartitions(partitionF);
    destroyPartitions(partitionG);
    destroyMappingOfClasses(mappingOfClassesF);
    destroyMappingOfClasses(mappingOfClassesG);
    freeTtLinkedList(l1);

    // End time
    clock_t endTotalTime = clock();
    totalTime += (double) (endTotalTime - startTotalTime) / CLOCKS_PER_SEC;

    printf("Original algorithm: \n");
    printf("Time spent partitioning: %f \n", partitionTime);
    printf("Time spent outer permutation: %f \n", outerPermutationTime);
    printf("Time spent inner permutation: %f \n", innerPermutationTime);
    printf("Total time spent: %f \n", totalTime);

    fprintf(fp, "\nOriginal algorithm:\n");
    fprintf(fp, "Time spent partitioning: %f \n", partitionTime);
    fprintf(fp, "Time spent outer permutation: %f \n", outerPermutationTime);
    fprintf(fp, "Time spent inner permutation: %f \n", innerPermutationTime);
    fprintf(fp, "Total time spent: %f \n", totalTime);

    return 0;
}
