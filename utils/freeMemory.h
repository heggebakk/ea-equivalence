#ifndef EA_EQUIVALENCE_FREEMEMORY_H
#define EA_EQUIVALENCE_FREEMEMORY_H

#include "structs.h"

void freeTruthTable(truthTable tt);

void freeBuckets(partitions *p);

void freePartition(partitions p);

void freePermutations(permutations *p);
#endif //EA_EQUIVALENCE_FREEMEMORY_H
