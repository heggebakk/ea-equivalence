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
#include "utils/runTime.h"

int runOriginal(truthTable *f, truthTable *g, size_t k, size_t dimension, size_t *basis, FILE *fp, RunTimes *runTimes);

int runWalshTransform(truthTable *f, truthTable *g, size_t k, size_t dimension, size_t *basis, FILE *fp,
                      RunTimes *runTimes);

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
    RunTimes *runTimeWalsh = initRunTimes();
    RunTimes *runTime = initRunTimes();

    // Parse files to truth tables
    clock_t startParsing = clock();
    truthTable *functionF = parseTruthTable(argv[1]);
    truthTable *functionG = getFunctionG(functionF);
    runTimeWalsh->parsing = stopTime(runTimeWalsh->parsing, startParsing);
    runTime->parsing = runTimeWalsh->parsing;


    char *filename = "result.txt";
    FILE *fp = fopen(filename, "w+");
    fprintf(fp, "%s\n", argv[1]);

    printTruthTable(functionF);
    printTruthTable(functionG);
    fprintf(fp, "// Dimension:\n%zu\n", functionF->dimension);
    writeTruthTable(functionF, fp, "F");
    writeTruthTable(functionG, fp, "G");
    printf("\n");
    size_t DIMENSION = functionF->dimension;
    size_t *basis = createStandardBasis(DIMENSION); // Standard basis

    // Solve with Walsh transform first:
    runWalshTransform(functionF, functionG, k, DIMENSION, basis, fp, runTimeWalsh);
    printf("\n");

    // Solve with new algorithm:
    runOriginal(functionF, functionG, k, DIMENSION, basis, fp, runTime);

    fclose(fp);
    free(basis);

    return 0;
}

int runWalshTransform(truthTable *f, truthTable *g, size_t k, size_t dimension, size_t *basis, FILE *fp,
                      RunTimes *runTimes) {
    fprintf(fp, "\n** WALSH TRANSFORM **\n");
    printf("Walsh transform\n");

    clock_t startTotalTime = clock();

    // Partition function f and g
    clock_t startPartitionTime = clock();
    partitions *partitionF = eaPartitionWalsh(f, k);
    partitions *partitionG = eaPartitionWalsh(g, k);

    runTimes->partition = stopTime(runTimes->partition, startPartitionTime);

    /* Walsh Transform specific part done */

    // We might end up in a situation where we have more than one mapping of the partitions from F and G.
    // In this case, we must try and fail. If we succeed, we can finish, otherwise we need to try again.
    MappingOfClasses *mappingOfClassesF = initMappingsOfClasses();
    MappingOfClasses *mappingOfClassesG = initMappingsOfClasses();
    ttNode *l1 = initTtNode();

    mapPartitionClasses(partitionG, partitionF, dimension, mappingOfClassesF);
    mapPartitionClasses(partitionF, partitionG, dimension, mappingOfClassesG);

    // Loop over all the mappings, if we find a solution, we break and finish.
    for (int m = 0; m < mappingOfClassesF->numOfMappings; ++m) {
        bool foundSolution = false;

        // Calculate Outer Permutation
        clock_t startOuterPermutationTime = clock();
        size_t numPermutations = outerPermutation(partitionF, partitionG, dimension, basis, l1,
                                                  mappingOfClassesG->mappings[m], mappingOfClassesG->domains[m], fp,
                                                  mappingOfClassesF->mappings[m]);
        runTimes->outerPermutation = stopTime(runTimes->outerPermutation, startOuterPermutationTime);

        // Calculate inner permutation
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
                runTimes->innerPermutation = stopTime(runTimes->innerPermutation, startInnerPermutationTime);

                // Find l
                truthTable *l = composeFunctions(l1Prime, lPrime);
                writeTruthTable(l1[i].data, fp, "l1");
                writeTruthTable(l2, fp, "l2");
                writeTruthTable(l, fp, "l");

                foundSolution = true;

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

        destroyPartitions(partitionF);
        destroyPartitions(partitionG);
        destroyMappingOfClasses(mappingOfClassesF);
        destroyMappingOfClasses(mappingOfClassesG);
        freeTtLinkedList(l1);
        // End time
        runTimes->total = stopTime(runTimes->total, startTotalTime);

        if (!foundSolution) break;
    }

    // Print time information
    printTimes(runTimes);

    fprintf(fp, "\nWalsh Transform:\n");
    writeTimes(runTimes, fp);
    return 0;
}

int runOriginal(truthTable *f, truthTable *g, size_t k, size_t dimension, size_t *basis, FILE *fp, RunTimes *runTimes) {
    fprintf(fp, "\n** NEW ALGORITHM **\n");
    printf("New algorithm\n");
    ttNode *l1 = initTtNode();

    // Start time
    clock_t startTotalTime = clock();

    // Partition function f and g
    clock_t startPartitionTime = clock();
    partitions *partitionF = partitionFunction(f, k);
    partitions *partitionG = partitionFunction(g, k);

    // We might end up in a situation where we have more than one mapping of the partitions from F and G.
    // In this case, we must try and fail. If we succeed, we can finish, otherwise we need to try again.
    MappingOfClasses *mappingOfClassesF = initMappingsOfClasses();
    MappingOfClasses *mappingOfClassesG = initMappingsOfClasses();
    mapPartitionClasses(partitionG, partitionF, dimension, mappingOfClassesF);
    mapPartitionClasses(partitionF, partitionG, dimension, mappingOfClassesG);

    runTimes->partition = stopTime(runTimes->partition, startPartitionTime);

    // We need a for loop to go through all the possible mappings. If we succeed, we're finished and breaks out.

    // Outer permutation
    clock_t startOuterPermutationTime = clock();
    size_t numPermutations = outerPermutation(partitionF, partitionG, dimension, basis, l1,
                                              mappingOfClassesG->mappings[0], mappingOfClassesG->domains[0], fp,
                                              mappingOfClassesF->mappings[0]);
    runTimes->outerPermutation = stopTime(runTimes->outerPermutation, startOuterPermutationTime);

    // Inner permutation
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
            runTimes->innerPermutation = stopTime(runTimes->innerPermutation, startInnerPermutation);

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
    runTimes->total = stopTime(runTimes->total, startTotalTime);
    printTimes(runTimes);

    fprintf(fp, "\nNew algorithm:\n");
    writeTimes(runTimes, fp);

    return 0;
}
