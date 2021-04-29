#include "utils/fileParser.h"
#include "functions/partition.h"
#include "functions/permutation.h"

typedef struct vbfTruthTable truthTable;
typedef struct vbfPartitions partitions;

int main(void) {
    char *fileF;
    fileF = "/home/marie/MasterUiB/ea-equivalence/src/resources/TT_library/dim6/q_6_1.tt";
    truthTable functionF = parseTruthTable(fileF);
    partitions partitionF = partitionFunction(&functionF, 4, 0);
    printf("\n");
    char *fileG;
    fileG = "/home/marie/MasterUiB/ea-equivalence/src/resources/TT_library/dim6/q_6_2.tt";
    truthTable functionG = parseTruthTable(fileG);
    partitions partitionG = partitionFunction(&functionG, 4, 0);

    outerPermutation(partitionF, partitionG);
    return 0;
}