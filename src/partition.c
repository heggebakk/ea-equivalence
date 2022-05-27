#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

void printPartitionHelp();

void setPartitionFlags(int argc, char *const *argv, TruthTable **functionF);

int main(int argc, char *argv[]) {
    TruthTable *functionF;
    setPartitionFlags(argc, argv, &functionF);

    Partition *partition = partitionFunction(functionF, 4);
    printPartition(partition);
    destroyPartitions(partition);
    destroyTruthTable(functionF);
    return 0;

}
void printPartitionHelp() {
    printf("Partition\n");
    printf("Find the partition of a function.\n")
    printf("Usage: partition [filename]\n");
    printf("filename = Path to file of a function F.\n");
    printf("\n");
}

void setPartitionFlags(int argc, char *const *argv, TruthTable **functionF) {
    if (argc < 2) {
        printPartitionHelp();
        exit(0);
    } else {
        *functionF = parseTruthTable(argv[1]);
    }
}

