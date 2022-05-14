#ifndef EA_EQUIVALENCE_STRUCTURES_H
#define EA_EQUIVALENCE_STRUCTURES_H

#include "stdio.h"

/**
 * In this file you will find all the structures that is needed and used for all the different structures.
 */

/**
 * A Truth Table of a function F.
 * Holds the dimension, n, and all the elements of the function (2^n elements)
 */
typedef struct TruthTable {
    size_t dimension;
    size_t *elements;
} TruthTable;


/**
 * Initialize a new Truth Table
 * @param n The n
 * @return A new empty truth table
 */
TruthTable *initTruthTable(size_t n);

/**
 * Destroy a Truth table
 * @param tt The truth table to be destroyed
 */
void destroyTruthTable(TruthTable *tt);

/**
 * Print out the information stored in the Truth Table struct to the console
 * @param tt The Truth Table to print out
 */
void printTruthTable(TruthTable *tt);

/**
 * Write a Truth Table to a file
 * @param tt The truth table
 * @param fp The file to write to
 * @param ttName The name of the truth table represented as a String
 */
void writeTruthTable(FILE *fp, TruthTable *tt);

/**
 * Parse a file to a Truth Table Struct
 * The first line of the file must be the numberOfClasses, the next line should be all the elements seperated with white space
 *
 * Example:
 * 6
 * 0 1 8 15 27 14 35 48 53 39 43 63 47 41 1 1 41 15 15 47 52 6 34 22 20 33 36 23 8 41 8 47 36 52 35 53 35 39 20 22 33 34 48 53 39 48 6 23 22 33 63 14 23 52 14 43 27 63 36 6 27 43 20 34
 *
 * @param filename The filename to parse
 * @return A Truth Table struct with the parsed data
 */
TruthTable *parseTruthTable(const char filename[64]);

#include "stdio.h"

#endif //EA_EQUIVALENCE_STRUCTURES_H
