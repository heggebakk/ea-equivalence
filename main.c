#include "utils/fileParser.h"
#include "functions/partition.h"
#include "functions/permutation.h"
#include "time.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Expected 2 arguments");
        return 1;
    }
    char *file1;
    char *file2;
    file1 = argv[1];
    file2 = argv[2];

    double timeSpentPartition = 0.0;
    clock_t startPartition = clock();
    truthTable functionF = parseTruthTable(file1);
    printf("\n");
    truthTable functionG = parseTruthTable(file2);

    partitions partitionF = partitionFunction(&functionF, 4, 0);
    partitions partitionG = partitionFunction(&functionG, 4, 0);
    printf("\n");
    clock_t endPartition = clock();
    timeSpentPartition += (double) (endPartition - startPartition) / CLOCKS_PER_SEC;
    printf("Time spent Partitioning: %f seconds\n", timeSpentPartition);

    freeTruthTable(functionF);
    freeTruthTable(functionG);

    printf("Results from partition: \n");
    for (int i = 0; i < partitionF.numBuckets; ++i) {
        printf("%zu: ", partitionF.buckets[i]->bucketSize);
        for (int j = 0; j < partitionF.buckets[i]->bucketSize; ++j) {
            printf("%zu ", partitionF.buckets[i]->elements[j]);
        }
        printf("\n");
    }

    outerPermutation(partitionF, partitionG);

    freeBuckets(&partitionF);
    freeBuckets(&partitionG);

    freePartition(partitionF);
    freePartition(partitionG);

    return 0;
}
