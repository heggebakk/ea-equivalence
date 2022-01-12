#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "time.h"
#include "utils/truthTable.h"
#include "utils/parser.h"
#include "functions/outerPermutation.h"
#include "newMain.h"
#include "utils/eaTest.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Expected at least 2 arguments!");
        return 1;
    }

    long k = 4;
    if (argv[2] != NULL) {
        char* p;
        errno = 0;
        k = strtol(argv[2], &p, 10);
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
//    truthTable *functionG = parseTruthTable(argv[2]);
    truthTable *functionG = getFunctionG(functionF);
    clock_t endParsing = clock();
    timeSpentParsing += (double) (endParsing - startParsing) / CLOCKS_PER_SEC;

    char *filename = "result.txt";
    FILE *fp = fopen(filename, "w+");
    fprintf(fp, "%s\n", argv[1]);

    printTruthTable(functionF);
    printTruthTable(functionG);
    fprintf(fp, "// Dimension:\n%zu\n", functionF->dimension);
    writeTruthTable(functionF, fp, "F");
    writeTruthTable(functionG, fp, "G");
    size_t DIMENSION = functionF->dimension;
    size_t *basis = createBasis(DIMENSION); // Standard basis

    // Solve with Walsh transform first:
    runWalshTransform(functionF, functionG, k, DIMENSION, basis, fp);

    printf("\n");

    // Solve with new algorithm:
    newAlgorithm(functionF, functionG, k, DIMENSION, basis, fp);
    printf("Time spent parsing files: %f\n", timeSpentParsing);
    printf("\n");

    fclose(fp);
    free(basis);

    return 0;
}