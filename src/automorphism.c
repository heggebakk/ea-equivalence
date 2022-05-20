#include <stdlib.h>
#include "structures.h"
#include "permutation.h"

void printAmHelp();

void setAmFlags(int argc, char *const *argv, TruthTable **functionF);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printAmHelp();
        return 0;
    }
    TruthTable *functionF;

    setAmFlags(argc, argv, &functionF);

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
            printf("%zu\n", dimension);
            printf("%zu\n", countTtNodes(l1));
            printTtNodes(l1);
            destroyTtNodes(l1);
            break;
        }
        destroyTtNodes(l1);
    }
    destroyTruthTable(functionF);
    destroyMappingOfBuckets(mappingOfClasses);
    destroyPartitions(partition);
    free(basis);
    return 0;
}

void setAmFlags(int argc, char *const *argv, TruthTable **functionF) {
    if (argc < 2) {
        printAmHelp();
        exit(0);
    } else {
        *functionF = parseTruthTable(argv[1]);
    }
}

void printAmHelp() {
    printf("Automorphism\n");
    printf("Usage: automorphism [filename]\n");
    printf("\n");
    printf("filename = the path to the file of a function F.\n");
}
