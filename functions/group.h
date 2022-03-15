#ifndef EA_EQUIVALENCE_GROUP_H
#define EA_EQUIVALENCE_GROUP_H

#include "../utils/linkedList.h"
#include <stdbool.h>

void checkFlags(int argc, char **argv);

TtNode * parsePermutationFile(char *input);

void findGroups(TtNode *permutations);

void compose(TruthTable *src, TruthTable *dest);

bool isIdentity(TruthTable *dest);

size_t standardBasis();

void printHelp();

#endif //EA_EQUIVALENCE_GROUP_H
