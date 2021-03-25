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

    tt.elements = (int) pow(2, tt.dimension);
    tt.array = (int *) malloc(sizeof(int) * tt.elements);
    printf("Truth table size: %zu \n", tt.elements);
    for (int i = 0; i < tt.elements; ++i) {
        fscanf(fp, "%d", &tt.array[i]);
    }
    fclose(fp);
    printf("Dimension: %zu\n", tt.dimension);
    printf("Truth table: ");
    for (int i = 0; i < tt.elements; ++i) {
        printf("%d ", tt.array[i]);
    }
    printf("\n");
    return tt;
}