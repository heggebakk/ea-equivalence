#include <stdlib.h>
#include "parser.h"

truthTable parseTruthTable(const char* filename) {
    // TODO: Move all print statements out to main function
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Requested file does not exists in system or is not found. \n");
        exit(1);
    }

    truthTable tt;
    fscanf(file, "%zd", &tt.dimension);
    size_t size = 1L << tt.dimension;
    tt.elements = malloc(sizeof(size_t) * size);
    tt.size = size;
    printf("Truth table size: %zu \n", tt.size);

    for (size_t i = 0; i < tt.size; ++i) {
        fscanf(file, "%zd", &tt.elements[i]);
    }
    fclose(file);

    printf("Dimension: %zu \n", tt.dimension);
    printf("Truth table: ");
    for (size_t i = 0; i < tt.size; ++i) {
        printf("%zu ", tt.elements[i]);
    }
    printf("\n");
    return tt;
}
