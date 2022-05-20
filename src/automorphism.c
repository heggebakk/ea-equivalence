#include <stdlib.h>
#include "structures.h"
#include "permutation.h"

void printAmHelp();

void setAmFlags(int argc, char *const *argv, char **pathFunctionF, char **writePath);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printAmHelp();
        return 0;
    }
    char *pathFunctionF;
    char *writePath;

    setAmFlags(argc, argv, &pathFunctionF, &writePath);

    FILE *fp = fopen(writePath, "w+");
    TruthTable *functionF = parseTruthTable(pathFunctionF);

    MappingOfBuckets *mappingOfClasses = initMappingsOfBuckets();
    Partition *partition = partitionFunction(functionF, 4);
    size_t dimension = functionF->n;
    size_t *basis = createStandardBasis(dimension);
    mapPartitionBuckets(partition, partition, dimension, mappingOfClasses);

    for (int m = 0; m < mappingOfClasses->numOfMappings; ++m) {
        TtNode *l1 = outerPermutation(partition, partition, dimension, basis, mappingOfClasses->mappings[m],
                         mappingOfClasses->mappings[m],
                         mappingOfClasses->domains[m]);
        if (l1->data != NULL) {
            fprintf(fp, "%zu\n", dimension);
            fprintf(fp, "%zu\n", countTtNodes(l1));
            writeTtNodes(l1, fp);
            destroyTtNodes(l1);
            break;
        }
        destroyTtNodes(l1);
    }
    destroyTruthTable(functionF);
    destroyMappingOfBuckets(mappingOfClasses);
    destroyPartitions(partition);
    free(basis);
    fclose(fp);
    return 0;
}

void setAmFlags(int argc, char *const *argv, char **pathFunctionF, char **writePath) {
    for (int i = 0; i < argc; ++i) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'h':
                    printAmHelp();
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

void printAmHelp() {
    printf("Automorphism\n");
    printf("Usage: automorphism [automorphism_options]\n");
    printf("Automorphism options:\n");
    printf("\t-f \t- Path to function F\n");
    printf("\t-h \t- Print help\n");
    printf("\t-w \t- Path to where the results should be written to.\n");
    printf("\n");
}
