#include <stdlib.h>
#include "parser.h"

/**
 * Parse a file to a Truth Table Struct
 * The first line of the file must be the dimension, the next line should be all the elements seperated with white space
 *
 * Example:
 * 6
 * 0 1 8 15 27 14 35 48 53 39 43 63 47 41 1 1 41 15 15 47 52 6 34 22 20 33 36 23 8 41 8 47 36 52 35 53 35 39 20 22 33 34 48 53 39 48 6 23 22 33 63 14 23 52 14 43 27 63 36 6 27 43 20 34
 *
 * @param filename The filename to parse
 * @return A Truth Table struct with the parsed data
 */
truthTable * parseTruthTable(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Requested file does not exists in system or is not found. \n");
        exit(1);
    }

    truthTable *tt = malloc(sizeof(truthTable));
    fscanf(file, "%zd", &tt->dimension);
    tt->elements = calloc(sizeof(size_t), 1L << tt->dimension);

    for (size_t i = 0; i < 1L << tt->dimension; ++i) {
        fscanf(file, "%zd", &tt->elements[i]);
    }
    fclose(file);
    return tt;
}

/**
 * Print out the information stored in the Truth Table struct to the console
 * @param tt The Truth Table to print out
 */
void printTruthTable(truthTable *tt) {
    printf("Dimension: %zu \n", tt->dimension);
    printf("Truth table: ");

    for (size_t i = 0; i < 1L << tt->dimension; ++i) {
        printf("%zu ", tt->elements[i]);
    }
    printf("\n");
}
