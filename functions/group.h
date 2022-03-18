#ifndef EA_EQUIVALENCE_GROUP_H
#define EA_EQUIVALENCE_GROUP_H

#include "../utils/linkedList.h"
#include <stdbool.h>

void checkFlags(int argc, char **argv);

void parsePermutationFile(char *input);

void findGroups(TruthTable *permutation);

void compose(TruthTable *src, TruthTable *dest);

bool isIdentity(TruthTable *dest);

size_t standardBasis();

void printHelp();

#endif //EA_EQUIVALENCE_GROUP_H
