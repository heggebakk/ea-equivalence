#ifndef EA_EQUIVALENCE_GROUP_H
#define EA_EQUIVALENCE_GROUP_H

#include <stdbool.h>
#include "structures.h"

/**
 * Check the arguments for flags
 * @param argc Number of arguments
 * @param argv List of the arguments
 */
void checkFlags(int argc, char **argv);

/**
 * Parse a file containing a permutation of a function
 * @param input The path to the permutation file
 */
void parsePermutationFile(char *input);

/**
 * Find the order of a function F. By composing the function with itself one will reach the identity at some point.
 * How many times one need to compose = the order.
 * @param src The function F to compose
 */
void findOrder(TruthTable *src);

/**
 * Compose two functions with each other and update the TruthTable
 * @param src Function to compose with
 * @param dest Function to compose and update
 */
void compose(TruthTable *src, TruthTable *dest);

/**
 * Check if a given function F is the identity, such that the list of elements = {1,2,3,...,2^n)
 * @param F Function to check for identity
 * @return True if the function is the identity, false otherwise.
 */
bool isIdentity(TruthTable *F);

/**
 * Create a standard basis: {2^0, 2^1, 2^2, ..., 2^n - 1}
 * @return A list with the standard basis.
 */
size_t standardBasis();

/**
 * Print out to the console info for how to use the program.
 */
void printGroupHelp();

#endif //EA_EQUIVALENCE_GROUP_H
