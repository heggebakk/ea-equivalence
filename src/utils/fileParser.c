#include "fileParser.h"
#include <stdio.h>
#include <stdlib.h>

truthTable parseTruthTable(const char* filename) {
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Requested file does not exists in system or not found. \n");
        exit(1);
    }

    truthTable tt;
    fscanf(fp, "%zu", &tt.dimension);

    size_t size = 1L << tt.dimension;
    tt.elements = (size_t *) malloc(sizeof(size_t) * size);
    tt.size = size;
    printf("Truth table bucketSize: %zu \n", tt.size);
    for (int i = 0; i < tt.size; ++i) {
        fscanf(fp, "%zu", &tt.elements[i]);
    }
    fclose(fp);

    printf("Dimension: %zu\n", tt.dimension);
    printf("Truth table: ");
    for (int i = 0; i < tt.size; ++i) {
        printf("%d ", tt.elements[i]);
    }
    printf("\n");
    return tt;
}
