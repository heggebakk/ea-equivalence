#include <stdio.h>
#include <stdbool.h>
#include "time.h"
#include "utils/structs.h"
#include "utils/parser.h"
#include "functions/partition.h"
#include "utils/freeMemory.h"
#include "functions/outerPermutation.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Expected 2 arguments");
        return 1;
    }
    char *truthTable1 = argv[1];
    char *truthTable2 = argv[2];

    double timeSpentParsing = 0.0;
    clock_t startParsing = clock();
    truthTable functionF = parseTruthTable(truthTable1);
    truthTable functionG = parseTruthTable(truthTable2);
    clock_t endParsing = clock();
    timeSpentParsing += (double) (endParsing - startParsing) / CLOCKS_PER_SEC;
    printTruthTableInfo(functionF);
    printTruthTableInfo(functionG);
    printf("\n");

    size_t k = 4;
    size_t t = 0;
    double timeSpentPartition = 0.0;
    clock_t startPartition = clock();
    partitions partitionF = partitionFunction(&functionF, k, t);
    partitions partitionG = partitionFunction(&functionG, k, t);
    clock_t endPartition = clock();
    timeSpentPartition += (double) (endPartition - startPartition) / CLOCKS_PER_SEC;
    freeTruthTable(functionF);
    freeTruthTable(functionG);

    printf("Results from partition function F: \n");
    printPartitionInfo(partitionF);
    printf("Results from partition function G: \n");
    printPartitionInfo(partitionG);
    printf("\n");

    double timeSpentPermutation = 0.0;
    clock_t startPermutation = clock();
    permutations outerPerm = outerPermutation(partitionF, partitionG, functionF.dimension);
    clock_t endPermutation = clock();
    timeSpentPermutation += (double) (endPermutation - startPermutation) / CLOCKS_PER_SEC;
    printf("Number of permutations: %zu \n", outerPerm.numPermutations);

    bool bijective = isBijective(outerPerm, functionF.dimension);
    printf("The permutations is bijective: %s \n", bijective ? "true" : "false");
    printf("\n");

    freePermutations(outerPerm);
    freePartition(partitionF);
    freePartition(partitionG);

    printf("Time spent parsing: %f seconds \n", timeSpentParsing);
    printf("Time spent partitioning: %f seconds \n", timeSpentPartition);
    printf("Time spent outer permutation: %f seconds\n", timeSpentPermutation);

    return 0;
}