#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "time.h"
#include "structures.h"
#include "permutation.h"
bool hybrid;
void
runAlgorithm(TruthTable *F, TruthTable *G, Partition *partitionF, Partition *partitionG, size_t n, RunTimes *runTime,
             size_t *basis);

/**
 * Shuffle a linked linked list
 * @param head The head of the linked list
 * @return A shuffled linked list
 */
TtNode * shuffle(TtNode *head);

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

/**
 * Create a random linear function of n n
 * @param n The n
 * @return A random linear function of n n
 */
TruthTable *randomLinearFunction(size_t n);

/**
 * Create a random linear permutation of n n
 * @param n The n
 * @return A random linear permutation of n n
 */
TruthTable *randomLinearPermutation(size_t n);

/**
 * Create a new function with respect to a function F
 * @param F The function F to create with respect to
 * @return A new function
 */
TruthTable * createFunction(TruthTable *F);

int main(int argc, char *argv[]) {
    TruthTable *functionF = NULL;
    TruthTable *functionG = NULL;
    long k = 4;
    RunTimes *runTime;
    bool times = false; // Set to true to print out running times
    Partition *partitionF;
    Partition *partitionG;
    clock_t startTotalTime;
    hybrid = true;

    setFlags(argc, argv, &times, &functionF, &functionG, &k);

    // Parse files to truth tables
    // Parse function F. Parse function G if given, otherwise create random function G with respect to function F.
    if (functionG == NULL) functionG = createFunction(functionF);

    size_t n = functionF->n;
    size_t *basis = createStandardBasis(n);

    for (int a = 0; a < 2; ++a) {
        // Solve with Walsh transform:
        if (a == 0) {
            startTotalTime = clock();
            runTime = initRunTimes();

            // Partition function f and g
            clock_t startPartitionTime = clock();
            partitionF = eaPartitionWalsh(functionF, k);
            partitionG = eaPartitionWalsh(functionG, k);
            runTime->partition = stopTime(runTime->partition, startPartitionTime);

            runAlgorithm(functionF, functionG, partitionF, partitionG, n,
                         runTime, basis);

            runTime->total = stopTime(runTime->total, startTotalTime); // End time

            if (times) {
                // Print running times
                printf("\nWalsh Transform:\n");
                printTimes(runTime);
            }
            destroyRunTimes(runTime);
            destroyPartitions(partitionF);
            destroyPartitions(partitionG);
        } else if (a == 1) {
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
                printf("\nNew Algorithm\n");
                printTimes(runTime);
            }
            destroyRunTimes(runTime);
            destroyPartitions(partitionF);
            destroyPartitions(partitionG);
        }
    }
    destroyTruthTable(functionF);
    destroyTruthTable(functionG);
    free(basis);

    return 0;
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
    printf("EA-equivalence test\n");
    printf("Usage: ea [ea_options] [filename_F] [filename_G]\n");
    printf("Ea options:\n");
    printf("\t-h \t - Print help\n");
    printf("\t-k \t - Size of k\n");
    printf("\t-t \t - Add this for printing running times for different functions.\n");
    printf("\n");
    printf("\tfilename_F: path to the file of a function F\n");
    printf("\tfilename_G: path to the file of a function G\n");
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
        if (hybrid) {
            hybridEquivalenceTest(partitionF, partitionG, n, basis, mappingOfBucketsF->mappings[m],
                                  mappingOfBucketsG->mappings[m], mappingOfBucketsG->domains[m], F, G, runTime);
            break;
        } else {


            bool foundSolution = false;

            // Calculate Outer Permutation
            clock_t startOuterPermutationTime = clock();
            TtNode *allL1 = outerPermutation(partitionF, partitionG, n, basis,
                                             mappingOfBucketsF->mappings[m],
                                             mappingOfBucketsG->mappings[m], mappingOfBucketsG->domains[m]);
            runTime->outerPermutation = stopTime(runTime->outerPermutation, startOuterPermutationTime);
            size_t numPermutations = countTtNodes(allL1);

            /* Shuffle list of permutations */
//        TtNode *L1Shuffled;
//        if (numPermutations > 0) {
//            L1Shuffled = shuffle(allL1);
//            destroyTtLinkedList(allL1);
//        } else continue;

            for (size_t i = 0; i < numPermutations; ++i) {
//            TruthTable *L1 = getNode(L1Shuffled, i);
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
//        destroyTtLinkedList(L1Shuffled);
        destroyTtNodes(allL1);
        if (foundSolution) break;
        }
    }
    destroyMappingOfBuckets(mappingOfBucketsF);
    destroyMappingOfBuckets(mappingOfBucketsG);
}

TtNode *shuffle(TtNode *head) {
    size_t n = countTtNodes(head);
    if (n <= 1) return head;

    TruthTable *ptr[n];
    // Add pointer of node to array
    for (size_t i = 0; i < n; ++i) {
        ptr[i] = getTtNode(head, i);
    }
    for (size_t i = 0; i < n - 1; ++i) {
        size_t rnd = (size_t) rand();
        size_t j = (i + rnd / (RAND_MAX / (n - 1) + 1)) % n;
        TruthTable *t = ptr[j];
        ptr[j] = ptr[i];
        ptr[i] = t;
    }
    TtNode *new = initTtNode();
    for (int i = 0; i < n; ++i) {
        addTtNode(new, ptr[i]);
    }
    return new;
}

TruthTable *randomLinearFunction(size_t n) {
    size_t entries = 1L << n;
    size_t listGenerated[entries];
    listGenerated[0] = 0;
    size_t basisImages[n];
    for (size_t i = 0; i < n; ++i) {
        size_t j = rand() % entries;
        basisImages[i] = j;
        for (int k = 0; k < 1L << i; ++k) {
            listGenerated[(1L << i) + k] = listGenerated[k] ^ j;
        }
    }
    TruthTable *result = initTruthTable(n);
    memcpy(result->elements, listGenerated, sizeof(size_t) * entries);
    return result;
}

TruthTable *randomLinearPermutation(size_t n) {
    size_t entries = 1L << n;
    bool *generated = calloc(sizeof (bool), entries);
    size_t listGenerated[entries];
    generated[0] = true;
    listGenerated[0] = 0;

    size_t basisImages[n];
    for (int i = 0; i < n; ++i) {
        size_t j = rand() % entries;
        while (generated[j]) {
            j = (j + 1) % entries;
        }
        basisImages[i] = j;
        for (int k = 0; k < 1L << i; ++k) {
            listGenerated[1L << i ^ k] = listGenerated[k] ^ j;
            generated[listGenerated[k] ^ j] = true;
        }
    }
    TruthTable *result = initTruthTable(n);
    memcpy(result->elements, listGenerated, sizeof(size_t) * entries);
    free(generated);
    return result;
}

TruthTable * createFunction(TruthTable *F) {
    size_t n = F->n;
    TruthTable *L1 = randomLinearPermutation(n);
    TruthTable *L2 = randomLinearPermutation(n);
    TruthTable *L = randomLinearFunction(n);

    TruthTable *temp = composeFunctions(F, L2);
    TruthTable *G = composeFunctions(L1, temp);
    addFunctionsTogether(G, L);

    destroyTruthTable(L1);
    destroyTruthTable(L2);
    destroyTruthTable(L);
    destroyTruthTable(temp);

    return G;
}