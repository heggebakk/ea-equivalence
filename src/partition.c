#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

void printPartitionHelp();
void setPartitionFlags(int argc, char *const *argv, char **pathFunctionF, char **writePath);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printPartitionHelp();
        return 0;
    }
    char *pathFunctionF;
    char *writePath;
    setPartitionFlags(argc, argv, &pathFunctionF, &writePath);

    FILE *fp = fopen(writePath, "w+");
    TruthTable *functionF = parseTruthTable(pathFunctionF);

    Partition *partition = partitionFunction(functionF, 4);
    writePartition(partition, fp);
    printPartitionInfo(partition);
    destroyPartitions(partition);
    destroyTruthTable(functionF);
    fclose(fp);
    return 0;

}
void printPartitionHelp() {
    printf("Partition\n");
    printf("Usage: partition [partition_options]\n");
    printf("Partition options:\n");
    printf("\t-f \t- Path to function F\n");
    printf("\t-h \t- Print help\n");
    printf("\t-w \t- Path to where the results should be written to.\n");
    printf("\n");
}

void setPartitionFlags(int argc, char *const *argv, char **pathFunctionF, char **writePath) {
    for (int i = 0; i < argc; ++i) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'h':
                    printPartitionHelp();
                    exit(0);
                case 'f':
                    i++;
                    *pathFunctionF = argv[i];
                    continue;
                case 'w':
                    i++;
                    *writePath = argv[i];
                    continue;
            }
        }
    }
}

