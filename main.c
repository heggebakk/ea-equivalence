#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "time.h"
#include "utils/truthTable.h"
#include "functions/outerPermutation.h"
#include "utils/eaTest.h"
#include "walshTransform/walshTransform.h"
#include "functions/innerPermutation.h"
#include "utils/runTime.h"

void runAlgorithm(TruthTable *functionF, TruthTable *functionG, partitions *partitionF, partitions *partitionG,
                  size_t DIMENSION, RunTimes *runTime, size_t *basis, TtNode *l1, FILE *fp);

/**
 * Create the inverse function
 * @param function The function to create the inverse of
 * @return The inverse
 */
TruthTable * inverse(TruthTable function);

int main(int argc, char *argv[]) {
    long k = 4;

    if (argc < 2) {
        printf("Expected at least 1 argument!");
        return 1;
    }

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
    clock_t startParsing= clock();
    double parsingTime = 0.0;
    TruthTable *functionF = parseTruthTable(argv[1]);
    TruthTable *functionG = getFunctionG(functionF);
    parsingTime += (double) (clock() - startParsing) / CLOCKS_PER_SEC;

    // Specify which file to write to.
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

    size_t *basis = createStandardBasis(DIMENSION);

    RunTimes *runTime;
    partitions *partitionF;
    partitions *partitionG;
    TtNode *l1 = initTtNode();
    clock_t startTotalTime;

    for (int a = 0; a < 2; ++a) {
        // Solve with Walsh transform first:
        if (a == 0) {
            startTotalTime = clock();
            runTime = initRunTimes();
            runTime->parsing = parsingTime;
            fprintf(fp, "\n** WALSH TRANSFORM **\n");
            printf("Walsh transform\n");

            // Partition function f and g
            clock_t startPartitionTime = clock();
            partitionF = eaPartitionWalsh(functionF, k);
            partitionG = eaPartitionWalsh(functionG, k);
            runTime->partition = stopTime(runTime->partition, startPartitionTime);

            runAlgorithm(functionF, functionG, partitionF, partitionG, DIMENSION,
                         runTime, basis, l1, fp);

            // End time
            runTime->total = stopTime(runTime->total, startTotalTime);
            // Print time information
            printTimes(runTime);

            fprintf(fp, "\nWalsh Transform:\n");
            writeTimes(runTime, fp);

            destroyRunTimes(runTime);
            destroyPartitions(partitionF);
            destroyPartitions(partitionG);
        } else if (a == 1) {
            startTotalTime = clock();
            runTime = initRunTimes();
            runTime->parsing = parsingTime;
            fprintf(fp, "\n** NEW ALGORITHM **\n");
            printf("\nNew algorithm\n");

            // Partition function f and g
            clock_t startPartitionTime = clock();
            partitionF = partitionFunction(functionF, k);
            partitionG = partitionFunction(functionG, k);
            runTime->partition = stopTime(runTime->partition, startPartitionTime);

            runAlgorithm(functionF, functionG, partitionF, partitionG, DIMENSION,
                         runTime, basis, l1, fp);

            // End time
            runTime->total = stopTime(runTime->total, startTotalTime);
            // Print time information
            printTimes(runTime);
            fprintf(fp, "\nOriginal Algorithm\n");
            writeTimes(runTime, fp);

            destroyRunTimes(runTime);
            destroyPartitions(partitionF);
            destroyPartitions(partitionG);
        }
    }
    destroyTruthTable(functionF);
    destroyTruthTable(functionG);
    free(basis);
    destroyTtLinkedList(l1);
    fclose(fp);

    return 0;
}

void runAlgorithm(TruthTable *functionF, TruthTable *functionG, partitions *partitionF, partitions *partitionG,
                  size_t DIMENSION, RunTimes *runTime, size_t *basis, TtNode *l1, FILE *fp) {
    // We might end up in a situation where we have more than one mapping of the partitions from F and G.
    // In this case, we must try and fail. If we succeed, we can finish, otherwise we need to try again.
    MappingOfClasses *mappingOfClassesF = initMappingsOfClasses();
    MappingOfClasses *mappingOfClassesG = initMappingsOfClasses();
    mapPartitionClasses(partitionG, partitionF, DIMENSION, mappingOfClassesF);
    mapPartitionClasses(partitionF, partitionG, DIMENSION, mappingOfClassesG);

    // Loop over all the mappings, if we find a solution, we break and finish.
    for (int m = 0; m < mappingOfClassesG->numOfMappings; ++m) {
        bool foundSolution = false;

        // Calculate Outer Permutation
        clock_t startOuterPermutationTime = clock();
        size_t numPermutations = outerPermutation(partitionF, partitionG, DIMENSION, basis, l1,
                                                  mappingOfClassesF->mappings[m],
                                                  mappingOfClassesG->mappings[m], mappingOfClassesG->domains[m], fp);
        runTime->outerPermutation = stopTime(runTime->outerPermutation, startOuterPermutationTime);

        // Calculate inner permutation
        clock_t startInnerPermutationTime = clock();
        for (size_t i = 0; i < numPermutations; ++i) {
            TruthTable *l1Prime = getNode(l1, i);
            TruthTable *l1Inverse = inverse(*l1Prime);
            TruthTable *gPrime = composeFunctions(l1Inverse, functionG);
            TruthTable *lPrime;
            TruthTable *l2 = initTruthTable(DIMENSION);

            if (innerPermutation(functionF, gPrime, basis, l2, &lPrime)) {
                runTime->innerPermutation = stopTime(runTime->innerPermutation, startInnerPermutationTime);

                // Find l
                TruthTable *l = composeFunctions(l1Prime, lPrime);
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

        if (!foundSolution) break;
    }
    destroyMappingOfClasses(mappingOfClassesF);
    destroyMappingOfClasses(mappingOfClassesG);
}

TruthTable *inverse(TruthTable function) {
    TruthTable *result = initTruthTable(function.dimension);
    for (size_t x = 0; x < 1L << function.dimension; ++x) {
        size_t y = function.elements[x];
        result->elements[y] = x;
    }
    return result;
}