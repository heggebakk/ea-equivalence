#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "time.h"
#include "structures.h"
#include "permutation.h"

void runAlgorithm(TruthTable *F, TruthTable *G, Partition *partitionF, Partition *partitionG,
                  size_t n, RunTimes *runTime, size_t *basis, FILE *fp);

void printEaHelp();

/**
 * Calculate the inverse of a function F
 * @param F The function F to inverse
 * @return The inverse of function F
 */
TruthTable *inverse(TruthTable F);

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
 * @param fileFunctionF Path to function F
 * @param fileFunctionG  Path to function G
 * @param k The number k, default is 4
 */
void setFlags(int argc, char *const *argv, char **filename, char **fileFunctionF, char **fileFunctionG, long *k);

/**
 * A helper that tells the user what kind of flags that can be used and what the program need to run
 */
void printEaHelp();

/**
 * Create a random linear function of dimension n
 * @param n The dimension
 * @return A random linear function of dimension n
 */
TruthTable *randomLinearFunction(size_t n);

/**
 * Create a random linear permutation of dimension n
 * @param n The dimension
 * @return A random linear permutation of dimension n
 */
TruthTable *randomLinearPermutation(size_t n);

/**
 * Create a new function with respect to a function F
 * @param F The function F to create with respect to
 * @return A new function
 */
TruthTable * createFunction(TruthTable *F);

int main(int argc, char *argv[]) {
    char *filename = "result.txt"; // Default position to write results to
    char *pathFunctionF; // Path to function F
    char *pathFunctionG = NULL; // Path to function G
    long k = 4;

    if (argc < 2) {
        printEaHelp();
        return 0;
    }

    setFlags(argc, argv, &filename, &pathFunctionF, &pathFunctionG, &k);

    // Specify which file to write to.
    FILE *fp = fopen(filename, "w+");

    // Parse files to truth tables
    // Parse function F. Parse function G if given, otherwise create random function G with respect to function F.
    TruthTable *functionF = parseTruthTable(pathFunctionF);
    TruthTable *functionG;
    if (pathFunctionG != NULL) functionG = parseTruthTable(pathFunctionG);
    else functionG = createFunction(functionF);

    printTruthTable(functionF);
    printTruthTable(functionG);
    size_t n = functionF->dimension;

    size_t *basis = createStandardBasis(n);

    RunTimes *runTime;
    Partition *partitionF;
    Partition *partitionG;
    clock_t startTotalTime;

    for (int a = 0; a < 2; ++a) {
        // Solve with Walsh transform:
        if (a == 0) {
            startTotalTime = clock();
            runTime = initRunTimes();
            printf("Walsh transform\n");

            // Partition function f and g
            clock_t startPartitionTime = clock();
            partitionF = eaPartitionWalsh(functionF, k);
            partitionG = eaPartitionWalsh(functionG, k);
            runTime->partition = stopTime(runTime->partition, startPartitionTime);

            runAlgorithm(functionF, functionG, partitionF, partitionG, n,
                         runTime, basis, fp);

            // End time
            runTime->total = stopTime(runTime->total, startTotalTime);
            // Print time information
            printTimes(runTime);

            fprintf(fp, "\nWalsh Transform:\n");
            writeTimes(runTime, fp);
            fprintf(fp, "\n");

            destroyRunTimes(runTime);
            destroyPartitions(partitionF);
            destroyPartitions(partitionG);
        } else if (a == 1) {
            startTotalTime = clock();
            runTime = initRunTimes();
            printf("\nNew algorithm\n");

            // Partition function f and g
            clock_t startPartitionTime = clock();
            partitionF = partitionFunction(functionF, k);
            partitionG = partitionFunction(functionG, k);
            runTime->partition = stopTime(runTime->partition, startPartitionTime);

            runAlgorithm(functionF, functionG, partitionF, partitionG, n,
                         runTime, basis, fp);

            // End time
            runTime->total = stopTime(runTime->total, startTotalTime);
            // Print time information
            printTimes(runTime);
            fprintf(fp, "\nNew Algorithm\n");
            writeTimes(runTime, fp);
            fprintf(fp, "\n");

            destroyRunTimes(runTime);
            destroyPartitions(partitionF);
            destroyPartitions(partitionG);
        }
    }
    destroyTruthTable(functionF);
    destroyTruthTable(functionG);
    free(basis);
    fclose(fp);

    return 0;
}

void setFlags(int argc, char *const *argv, char **filename, char **fileFunctionF, char **fileFunctionG, long *k) {
    for (int i = 1; i < argc; ++i) {
        // Check for flag
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                // Help
                case 'h':
                    printEaHelp();
                    exit(0);
                case 'f':
                    // Parse function F to TruthTable
                    i++;
                    (*fileFunctionF) = argv[i];
                    continue;
                case 'g':
                    // Parse function G to TruthTable
                    i++;
                    (*fileFunctionG) = argv[i];
                    continue;
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
                case 'w':
                    i++;
                    (*filename) = argv[i];
                    continue;
            }
        }
    }
}

void printEaHelp() {
    printf("EA-equivalence test\n");
    printf("Usage: ea [ea_options]\n");
    printf("Ea options:\n");
    printf("\t-f \t - Path to Function F\n");
    printf("\t-g \t - Path to Function G\n");
    printf("\t-h \t - Print help\n");
    printf("\t-k \t - Size of k\n");
    printf("\t-p \t - Calculate partitioning, set this if you want to calculate the partition of a function.\n");
    printf("\t-w \t - The path to where the results should be written to.\n");
    printf("\n");
}

void runAlgorithm(TruthTable *F, TruthTable *G, Partition *partitionF, Partition *partitionG,
                  size_t n, RunTimes *runTime, size_t *basis, FILE *fp) {
    // We might end up in a situation where we have more than one mapping of the Partition from F and G.
    // In this case, we must try and fail. If we succeed, we can finish, otherwise we need to try again.
    MappingOfBuckets *mappingOfClassesF = initMappingsOfClasses();
    MappingOfBuckets *mappingOfClassesG = initMappingsOfClasses();
    mapPartitionClasses(partitionG, partitionF, n, mappingOfClassesF);
    mapPartitionClasses(partitionF, partitionG, n, mappingOfClassesG);

    // Loop over all the mappings, if we find a solution, we break and finish.
    for (int m = 0; m < mappingOfClassesG->numOfMappings; ++m) {
        bool foundSolution = false;

        // Calculate Outer Permutation
        clock_t startOuterPermutationTime = clock();
        TtNode *allL1 = outerPermutation(partitionF, partitionG, n, basis,
                                         mappingOfClassesF->mappings[m],
                                         mappingOfClassesG->mappings[m], mappingOfClassesG->domains[m]);
        runTime->outerPermutation = stopTime(runTime->outerPermutation, startOuterPermutationTime);
        size_t numPermutations = countTtNodes(allL1);

        /* Shuffle list of permutations */
        TtNode *L1Shuffled = shuffle(allL1);
        destroyTtLinkedList(allL1);

        fprintf(fp, "Permutations: %zu\n", numPermutations);
        printf("Permutations: %zu\n", numPermutations);
        for (size_t i = 0; i < numPermutations; ++i) {
            TruthTable *L1 = getNode(L1Shuffled, i);
            printTruthTable(L1);
            TruthTable *L1Inverse = inverse(*L1);
            TruthTable *currentG = composeFunctions(L1Inverse, G);
            TruthTable *LPrime;
            TruthTable *L2 = initTruthTable(n);

            // Calculate inner permutation
            clock_t startInnerPermutationTime = clock();
            if (innerPermutation(F, currentG, basis, L2, &LPrime)) {
                runTime->innerPermutation = stopTime(runTime->innerPermutation, startInnerPermutationTime);
                foundSolution = true;

                // Find L
                TruthTable *L = composeFunctions(L1, LPrime);
                writeTruthTable(fp, L);

                // Free memory
                destroyTruthTable(L);
                destroyTruthTable(L1Inverse);
                destroyTruthTable(L2);
                destroyTruthTable(LPrime);
                destroyTruthTable(currentG);

                break;
            }
            destroyTruthTable(L1Inverse);
            destroyTruthTable(L2);
            destroyTruthTable(currentG);
        }
        destroyTtLinkedList(L1Shuffled);
        if (foundSolution) break;
    }
    destroyMappingOfClasses(mappingOfClassesF);
    destroyMappingOfClasses(mappingOfClassesG);
}

TruthTable *inverse(TruthTable F) {
    TruthTable *result = initTruthTable(F.dimension);
    for (size_t x = 0; x < 1L << F.dimension; ++x) {
        size_t y = F.elements[x];
        result->elements[y] = x;
    }
    return result;
}

TtNode *shuffle(TtNode *head) {
    size_t n = countTtNodes(head);
    if (n <= 1) return head;

    TruthTable *ptr[n];
    // Add pointer of node to array
    for (size_t i = 0; i < n; ++i) {
        ptr[i] = getNode(head, i);
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
        addNode(new, ptr[i]);
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
    return result;
}

TruthTable * createFunction(TruthTable *F) {
    size_t n = F->dimension;
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