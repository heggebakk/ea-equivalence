#include "utils/fileParser.h"
#include "functions/partition.h"
#include "functions/permutation.h"
#include "time.h"

int main(void) {
    double timeSpentPartition = 0.0;
    clock_t startPartition = clock();

    char *filename;
    filename = "resources/TT_library/dim6/q_6_1.tt";
    truthTable functionF = parseTruthTable(filename);
    partitions partitionF = partitionFunction(&functionF, 4, 0);
    freeTruthTable(functionF);

    printf("\n");
    char *fileG;
    fileG = "resources/TT_library/dim6/q_6_1.tt";
    truthTable functionG = parseTruthTable(fileG);
    partitions partitionG = partitionFunction(&functionG, 4, 0);
    freeTruthTable(functionG);

    clock_t endPartition = clock();
    timeSpentPartition += (double) (endPartition - startPartition) / CLOCKS_PER_SEC;

    double timeSpentPermutation = 0.0;
    clock_t startPermutation = clock();
    outerPermutation(partitionF, partitionG);
    clock_t endPermutation = clock();
    timeSpentPermutation += (double) (endPermutation - startPermutation) / CLOCKS_PER_SEC;

    freeBuckets(&partitionF);
    freeBuckets(&partitionG);

    freePartition(partitionF);
    freePartition(partitionG);

    printf("Time spent Partitioning: %f seconds\n", timeSpentPartition);
    printf("Time spent Permutation: %f seconds\n", timeSpentPermutation);

    return 0;
}
