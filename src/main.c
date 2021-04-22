#include "utils/fileParser.h"
#include "functions/partition.h"

typedef struct vbfTruthTable truthTable;
typedef struct vbfPartitions partitions;

int main(void) {
    char *filename;
    filename = "/home/marie/MasterUiB/ea-equivalence/src/resources/TT_library/dim6/q_6_1.tt";
    truthTable tt = parseTruthTable(filename);
    partitions partition = partitionFunction(&tt, 4, 0);
    return 0;
}