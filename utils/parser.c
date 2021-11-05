#include <stdlib.h>
#include "parser.h"

truthTable parseTruthTable(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Requested file does not exists in system or is not found. \n");
        exit(1);
    }

    truthTable tt;
    fscanf(file, "%zd", &tt.dimension);
    tt.elements = calloc(sizeof(size_t), 1L << tt.dimension);

    for (size_t i = 0; i < 1L << tt.dimension; ++i) {
        fscanf(file, "%zd", &tt.elements[i]);
    }
    fclose(file);
    return tt;
}

void printTruthTable(truthTable tt) {
    printf("Dimension: %zu \n", tt.dimension);
    printf("Truth table: ");

    for (size_t i = 0; i < 1L << tt.dimension; ++i) {
        printf("%zu ", tt.elements[i]);
    }
    printf("\n");
}
