#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "time.h"
#include "functions/outerPermutation.h"
#include "utils/eaTest.h"
#include "functions/walshTransform.h"
#include "utils/runTime.h"
#include "structures.h"
#include "permutation.h"

void runAlgorithm(TruthTable *functionF, TruthTable *functionG, Partition *partitionF, Partition *partitionG,
                  size_t DIMENSION, RunTimes *runTime, size_t *basis, FILE *fp);

/**
 * Create the inverse function
 * @param function The function to create the inverse of
 * @return The inverse
 */
TruthTable *inverse(TruthTable function);

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
 * @param partitionOnly Is set to True if one is to only run the partitioning of a function
 * @param autoMorphism Is set to true if one want to calculate the automorphism of a function
 */
void setFlags(int argc, char *const *argv, char **filename, char **fileFunctionF, char **fileFunctionG, long *k,
              bool *partitionOnly, bool *autoMorphism);

/**
 * A helper that tells the user what kind of flags that can be used and what the program need to run
 */
void printHelp();

int main(int argc, char *argv[]) {
    char *filename = "result.txt"; // Default position to write results to
    char *fileFunctionF; // Path to function F
    char *fileFunctionG = NULL; // Path to function G
    long k = 4;
    bool partitionOnly = false; // Set to true if one want to run only the partitioning of a function
    bool autoMorphism = false; // Set to true if one want to run the automorphism of a function

    if (argc < 2) {
        printHelp();
        return 1;
    }

    setFlags(argc, argv, &filename, &fileFunctionF, &fileFunctionG, &k, &partitionOnly, &autoMorphism);

    // Specify which file to write to.
    FILE *fp = fopen(filename, "w+");

    // Parse files to truth tables
    // Parse function F. Parse function G if given, otherwise create random function G with respect to function F.
    TruthTable *functionF = parseTruthTable(fileFunctionF);
    TruthTable *functionG;
    if (fileFunctionG != NULL) functionG = parseTruthTable(fileFunctionG);
    else functionG = getFunctionG(functionF);

    if (partitionOnly) {
        Partition *partition = partitionFunction(functionF, k);
        writePartition(partition, fp);
        printPartitionInfo(partition);
        destroyPartitions(partition);
        destroyTruthTable(functionF);
        destroyTruthTable(functionG);
        fclose(fp);
        return 0;
    }
    if (autoMorphism) {
        MappingOfBuckets *mappingOfClasses = initMappingsOfClasses();
        Partition *partition = partitionFunction(functionF, k);
        size_t dimension = functionF->dimension;
        size_t *basis = createStandardBasis(dimension);
        mapPartitionClasses(partition, partition, dimension, mappingOfClasses);

        for (int m = 0; m < mappingOfClasses->numOfMappings; ++m) {
            TtNode *l1 = initTtNode();
            outerPermutation(partition, partition, dimension, basis, mappingOfClasses->mappings[m],
                             mappingOfClasses->mappings[m],
                             mappingOfClasses->domains[m]);
            if (l1->data != NULL) {
                fprintf(fp, "%zu\n", dimension);
                fprintf(fp, "%zu\n", countTtNodes(l1));
                writeTtLinkedList(l1, fp);
                destroyTtLinkedList(l1);
                break;
            }
            destroyTtLinkedList(l1);
        }
        destroyTruthTable(functionF);
        destroyTruthTable(functionG);
        destroyMappingOfClasses(mappingOfClasses);
        destroyPartitions(partition);
        free(basis);
        fclose(fp);
        return 0;
    }

    printTruthTable(functionF);
    printTruthTable(functionG);
    size_t DIMENSION = functionF->dimension;

    size_t *basis = createStandardBasis(DIMENSION);

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

            runAlgorithm(functionF, functionG, partitionF, partitionG, DIMENSION,
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

            runAlgorithm(functionF, functionG, partitionF, partitionG, DIMENSION,
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

void setFlags(int argc, char *const *argv, char **filename, char **fileFunctionF, char **fileFunctionG, long *k,
              bool *partitionOnly, bool *autoMorphism) {
    for (int i = 1; i < argc; ++i) {
        // Check for flag
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                // Help
                case 'h':
                    printHelp();
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
                case 'o':
                    (*autoMorphism) = true;
                    continue;
                case 'w':
                    i++;
                    (*filename) = argv[i];
                    continue;
                case 'p': {
                    (*partitionOnly) = true;
                }
            }
        }
    }
}

void printHelp() {
    printf("EA-equivalence test\n");
    printf("Usage: ea [ea_options]\n");
    printf("Ea_options:\n");
    printf("\t-f \t - Path to Function F\n");
    printf("\t-g \t - Path to Function G\n");
    printf("\t-h \t - Print help\n");
    printf("\t-k \t - Size of k\n");
    printf("\t-o \t - Calculate auto morphism, set this if you want to calculate the automorphisms of a function.\n");
    printf("\t-p \t - Calculate partitioning, set this if you want to calculate the partition of a function.\n");
    printf("\t-w \t - The path to where the results should be written to.\n");
    printf("\n");
}

void runAlgorithm(TruthTable *functionF, TruthTable *functionG, Partition *partitionF, Partition *partitionG,
                  size_t DIMENSION, RunTimes *runTime, size_t *basis, FILE *fp) {
    // We might end up in a situation where we have more than one mapping of the Partition from F and G.
    // In this case, we must try and fail. If we succeed, we can finish, otherwise we need to try again.
    MappingOfBuckets *mappingOfClassesF = initMappingsOfClasses();
    MappingOfBuckets *mappingOfClassesG = initMappingsOfClasses();
    mapPartitionClasses(partitionG, partitionF, DIMENSION, mappingOfClassesF);
    mapPartitionClasses(partitionF, partitionG, DIMENSION, mappingOfClassesG);

    // Loop over all the mappings, if we find a solution, we break and finish.
    for (int m = 0; m < mappingOfClassesG->numOfMappings; ++m) {
        bool foundSolution = false;

        // Calculate Outer Permutation
        clock_t startOuterPermutationTime = clock();
        TtNode *l1 = outerPermutation(partitionF, partitionG, DIMENSION, basis,
                                      mappingOfClassesF->mappings[m],
                                      mappingOfClassesG->mappings[m], mappingOfClassesG->domains[m]);
        runTime->outerPermutation = stopTime(runTime->outerPermutation, startOuterPermutationTime);
        size_t numPermutations = countTtNodes(l1);

        /* Shuffle list of permutations */
        TtNode *l1Shuffled = shuffle(l1);
        destroyTtLinkedList(l1);

        // Calculate inner permutation
        clock_t startInnerPermutationTime = clock();
        fprintf(fp, "Permutations: %zu\n", numPermutations);
        printf("Permutations: %zu\n", numPermutations);
        for (size_t i = 0; i < numPermutations; ++i) {
            TruthTable *l1Prime = getNode(l1Shuffled, i);
            printTruthTable(l1Prime);
            TruthTable *l1Inverse = inverse(*l1Prime);
            TruthTable *gPrime = composeFunctions(l1Inverse, functionG);
            TruthTable *lPrime;
            TruthTable *l2 = initTruthTable(DIMENSION);

            if (innerPermutation(functionF, gPrime, basis, l2, &lPrime)) {
                foundSolution = true;

                runTime->innerPermutation = stopTime(runTime->innerPermutation, startInnerPermutationTime);

                // Find l
                TruthTable *l = composeFunctions(l1Prime, lPrime);
                writeTruthTable(fp, l);

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
        destroyTtLinkedList(l1Shuffled);
        if (foundSolution) break;
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

