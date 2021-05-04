#include <stdlib.h>
#include "utils/fileParser.h"
#include "functions/partition.h"
#include "functions/permutation.h"

typedef struct vbfTruthTable truthTable;
typedef struct vbfPartitions partitions;

int main(void) {
    char *filename;
    //filename = "/home/marie/MasterUiB/ea-equivalence/src/resources/TT_library/dim6/q_6_1.tt";
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

    outerPermutation(partitionF, partitionG);

    freeBuckets(&partitionF);
    freeBuckets(&partitionG);

    freePartition(partitionF);
    freePartition(partitionG);

    return 0;
}
