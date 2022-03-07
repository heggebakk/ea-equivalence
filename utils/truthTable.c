#include <stdlib.h>
#include "truthTable.h"

TruthTable * initTruthTable(size_t dimension) {
    TruthTable *newTt = malloc(sizeof(TruthTable));
    newTt->dimension = dimension;
    newTt->elements = malloc(sizeof(size_t) * 1L << dimension);
    return newTt;
}

void destroyTruthTable(TruthTable *tt) {
    free(tt->elements);
    free(tt);
}

void printTruthTable(TruthTable *tt) {
    printf("Dimension: %zu \n", tt->dimension);
    printf("Truth table: ");

    for (size_t i = 0; i < 1L << tt->dimension; ++i) {
        printf("%zu ", tt->elements[i]);
    }
    printf("\n");
}

void writeTruthTable(TruthTable *tt, FILE *fp, char *ttName) {
    fprintf(fp, "// %s:\n", ttName);

    for (size_t i = 0; i < 1L << tt->dimension; ++i) {
        fprintf(fp, "%zu ", tt->elements[i]);
    }
    fprintf(fp, "\n");
}

TruthTable * parseTruthTable(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Requested file does not exists in system or is not found. \n");
        exit(1);
    }

    size_t dimension;
    fscanf(file, "%zd", &dimension);
    TruthTable *tt = initTruthTable(dimension);

    for (size_t i = 0; i < 1L << tt->dimension; ++i) {
        fscanf(file, "%zd", &tt->elements[i]);
    }
    fclose(file);
    return tt;
}