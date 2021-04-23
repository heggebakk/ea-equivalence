#include "fileParser.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct vbfTruthTable truthTable;

truthTable parseTruthTable(const char* filename) {
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Requested file does not exists in system or not found. \n");
        exit(1);
    }

    truthTable tt;
    fscanf(fp, "%zd", &tt.dimension);

    size_t size = (size_t) pow(2, (double) tt.dimension);
    tt.elements = (int *) malloc(sizeof(int) * size);
    tt.size = size;
    printf("Truth table size: %zu \n", tt.size);
    for (int i = 0; i < tt.size; ++i) {
        fscanf(fp, "%d", &tt.elements[i]);
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