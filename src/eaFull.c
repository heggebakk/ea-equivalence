#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "time.h"
#include "structures.h"
#include "permutation.h"

/**
 * Here you will find the implementation of the algorithm for running the full version, where we fist find all outer
 * permutations, before we try to find an inner permutation.
 */

/**
 * Run the algorithm for finding a EA-equivalence between functions F and G
 * @param F Function F
 * @param G Function G
 * @param partitionF Partition of function F
 * @param partitionG Partition fo function G
 * @param n Dimension
 * @param runTime The run times for different functions
 * @param basis The basis
 */
void runAlgorithm(TruthTable *F, TruthTable *G, Partition *partitionF, Partition *partitionG, size_t n,
                  RunTimes *runTime, size_t *basis);

/**
 * All the flags that is or can be used to run the program
 * @param argc Number of program arguments
 * @param argv Program arguments
 * @param filename Path to where to write results to
 * @param functionF Path to function F
 * @param functionG  Path to function G
 * @param k The number k, default is 4
 */
void setFlags(int argc, char *const *argv, bool *times, TruthTable **functionF, TruthTable **functionG, long *k);

/**
 * A helper that tells the user what kind of flags that can be used and what the program need to run
 */
void printEaHelp();

int main(int argc, char *argv[]) {
    TruthTable *functionF = NULL;
    TruthTable *functionG = NULL;
    long k = 4;
    RunTimes *runTime;
    bool times = false; // Set to true to print out running times
    Partition *partitionF;
    Partition *partitionG;
    clock_t startTotalTime;

    setFlags(argc, argv, &times, &functionF, &functionG, &k);

    // Parse files to truth tables
    // Parse function F. Parse function G if given, otherwise create random function G with respect to function F.
    if (functionG == NULL){
        functionG = createFunction(functionF);
        printf("G:\n");
        printTruthTable(functionG);
    }
    size_t n = functionF->n;
    size_t *basis = createStandardBasis(n);

    if (k == 4) {
        // Solve with k-tuples
        startTotalTime = clock();
        runTime = initRunTimes();

        // Partition function f and g
        clock_t startPartitionTime = clock();
        partitionF = partitionFunction(functionF, k);
        partitionG = partitionFunction(functionG, k);
        runTime->partition = stopTime(runTime->partition, startPartitionTime);

        runAlgorithm(functionF, functionG, partitionF, partitionG, n,
                     runTime, basis);

        runTime->total = stopTime(runTime->total, startTotalTime); // End time

        if (times) {
            // Print running times
            printf("Running times:\n");
            printTimes(runTime);
        }
        destroyRunTimes(runTime);
        destroyPartitions(partitionF);
        destroyPartitions(partitionG);
    } else {
        // Solve with Walsh Transform
        startTotalTime = clock();
        runTime = initRunTimes();

        // Partition function f and g
        clock_t startPartitionTime = clock();
        partitionF = eaPartitionWalsh(functionF, k);
        partitionG = eaPartitionWalsh(functionG, k);
        runTime->partition = stopTime(runTime->partition, startPartitionTime);

        runAlgorithm(functionF, functionG, partitionF, partitionG, n, runTime, basis);

        runTime->total = stopTime(runTime->total, startTotalTime); // End time

        if (times) {
            // Print running times
            printf("Walsh Transform:\n");
            printTimes(runTime);
        }
        destroyRunTimes(runTime);
        destroyPartitions(partitionF);
        destroyPartitions(partitionG);
    }
    destroyTruthTable(functionF);
    destroyTruthTable(functionG);
    free(basis);

    return 0;
}


void
runAlgorithm(TruthTable *F, TruthTable *G, Partition *partitionF, Partition *partitionG, size_t n, RunTimes *runTime,
             size_t *basis) {
    // We might end up in a situation where we have more than one mapping of the Partition from F and G.
    // In this case, we must try and fail. If we succeed, we can finish, otherwise we need to try again.
    MappingOfBuckets *mappingOfBucketsF = initMappingsOfBuckets();
    MappingOfBuckets *mappingOfBucketsG = initMappingsOfBuckets();
    mapPartitionBuckets(partitionG, partitionF, n, mappingOfBucketsF);
    mapPartitionBuckets(partitionF, partitionG, n, mappingOfBucketsG);

    // Loop over all the mappings, if we find a solution, we break and finish.
    for (int m = 0; m < mappingOfBucketsG->numOfMappings; ++m) {
        bool foundSolution = false;

        // Calculate Outer Permutation
        clock_t startOuterPermutationTime = clock();
        TtNode *allL1 = outerPermutation(partitionF, partitionG, n, basis,
                                         mappingOfBucketsF->mappings[m],
                                         mappingOfBucketsG->mappings[m], mappingOfBucketsG->domains[m]);
        runTime->outerPermutation = stopTime(runTime->outerPermutation, startOuterPermutationTime);
        size_t numPermutations = countTtNodes(allL1);
        printf("Permutations: %zu\n", numPermutations);

        for (size_t i = 0; i < numPermutations; ++i) {
            TruthTable *L1 = getTtNode(allL1, i); // The current L1 from the outer permutations
            TruthTable *L1Inverse = inverse(L1);
            TruthTable *currentG = composeFunctions(L1Inverse, G);
            TruthTable *L;
            TruthTable *A2 = initTruthTable(n);

            // Calculate inner permutation
            clock_t startInnerPermutationTime = clock();
            if (innerPermutation(F, currentG, basis, A2, &L)) {
                runTime->innerPermutation = stopTime(runTime->innerPermutation, startInnerPermutationTime);
                foundSolution = true;
                // Find A, such that A = L1 * F * A2 + G
                TruthTable *A = composeFunctions(L1, L);

                // Print L1, A2 and A
                printf("L1:\n");
                printTruthTable(L1);
                printf("\nA2:\n");
                printTruthTable(A2);
                printf("\nA:\n");
                printTruthTable(A);
                printf("\n");

                // Free memory
                destroyTruthTable(A);
                destroyTruthTable(L1Inverse);
                destroyTruthTable(A2);
                destroyTruthTable(L);
                destroyTruthTable(currentG);

                break;
            }
            destroyTruthTable(L1Inverse);
            destroyTruthTable(A2);
            destroyTruthTable(currentG);
        }
        destroyTtNodes(allL1);
        if (foundSolution) break;
    }
    destroyMappingOfBuckets(mappingOfBucketsF);
    destroyMappingOfBuckets(mappingOfBucketsG);
}

void setFlags(int argc, char *const *argv, bool *times, TruthTable **functionF, TruthTable **functionG, long *k) {
    if (argc < 2) {
        printEaHelp();
        exit(0);
    }

    for (int i = 1; i < argc; ++i) {
        // Check for flag
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                // Help
                case 'h':
                    printEaHelp();
                    exit(0);
                case 'k':
                    // Set the k
                    i++;
                    errno = 0;
                    char *p;
                    (*k) = strtol(argv[i], &p, 10);
                    if (*p != '\0' || errno != 0) {
                        printf("Conversion went wrong for k\n");
                        exit(1);
                    }
                    continue;
                case 't':
                    i++;
                    *times = true;
                    continue;
            }
        } else {
            if (*functionF == NULL) {
                *functionF = parseTruthTable(argv[i]);
            } else if (*functionG == NULL) {
                *functionG = parseTruthTable(argv[i]);
            }
        }
    }
}

void printEaHelp() {
    printf("EA-equivalence test - full version\n");
    printf("Finds all outer permutations L1 first\n\n");
    printf("Usage: eaFull [eaFull_options] [filename_F] [filename_G]\n");
    printf("EaFull options:\n");
    printf("\t-h \t - Print help\n");
    printf("\t-k \t - Size of k\n");
    printf("\t-t \t - Add this for printing running times for different functions.\n");
    printf("\n");
    printf("\tfilename_F: path to the file of a function F\n");
    printf("\tfilename_G: path to the file of a function G\n");
}