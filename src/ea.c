#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "time.h"
#include "structures.h"
#include "permutation.h"
/**
 * Here you will find the implementation of the algorithm for running the efficient implementation, where for each outer
 * permutation we try to find an inner permutation
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
void
runAlgorithm(TruthTable *F, TruthTable *G, Partition *partitionF, Partition *partitionG, size_t n, RunTimes *runTime,
             size_t *basis);

/**
 * All the flags that is or can be used to run the program
 * @param argc
 * @param argv
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
    if (functionG == NULL) {
        functionG = createFunction(functionF);
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
        // Solve with walsh
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
    bool foundSolution = false;

    // Loop over all the mappings, if we find a solution, we break and finish.
    for (int m = 0; m < mappingOfBucketsG->numOfMappings; ++m) {
        foundSolution = hybridEquivalenceTest(partitionF, partitionG, n, basis, mappingOfBucketsF->mappings[m],
                                              mappingOfBucketsG->mappings[m], mappingOfBucketsG->domains[m], F, G, runTime);
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
                printf("%s\n", argv[i]);
            } else if (*functionG == NULL) {
                *functionG = parseTruthTable(argv[i]);
            }
        }
    }
}

void printEaHelp() {
    printf("EA-equivalence test\n");
    printf("Usage: ea[ea_options] [filename_F] [filename_G]\n");
    printf("Ea options:\n");
    printf("\t-h \t - Print help\n");
    printf("\t-k \t - Size of k\n");
    printf("\t-t \t - Add this for printing running times for different functions.\n");
    printf("\n");
    printf("\tfilename_F: path to the file of a function F\n");
    printf("\tfilename_G: path to the file of a function G\n");
}
