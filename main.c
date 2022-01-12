#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "time.h"
#include "utils/truthTable.h"
#include "utils/parser.h"
#include "functions/outerPermutation.h"
#include "newMain.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Expected at least 2 arguments!");
        return 1;
    }

    long k = 4;
    if (argv[3] != NULL) {
        char* p;
        errno = 0;
        k = strtol(argv[3], &p, 10);
        if (*p != '\0' || errno != 0) {
            printf("Conversion went wrong!\n");
            return 1;
        }
        printf("k = %ld\n", k);
    }

    // Parse files to truth tables
    double timeSpentParsing = 0.0;
    clock_t startParsing = clock();
    truthTable *functionF = parseTruthTable(argv[1]);
    truthTable *functionG = parseTruthTable(argv[2]);
    clock_t endParsing = clock();
    timeSpentParsing += (double) (endParsing - startParsing) / CLOCKS_PER_SEC;
    printTruthTable(functionF);
    printTruthTable(functionG);
    printf("Time spent parsing files: %f\n", timeSpentParsing);
    printf("\n");
    size_t DIMENSION = functionF->dimension;
    size_t *basis = createBasis(DIMENSION); // Standard basis

    // Solve with Walsh transform first:
    runWalshTransform(functionF, functionG, k, DIMENSION, basis);

    printf("\n");

    // Solve with new algorithm:
    newAlgorithm(functionF, functionG, k, DIMENSION, basis);

    free(basis);

    return 0;
}