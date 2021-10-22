#ifndef EA_EQUIVALENCE_FREEMEMORY_H
#define EA_EQUIVALENCE_FREEMEMORY_H

#include "structs.h"
#include "linkedList.h"

void freeTruthTable(truthTable tt);

void freePartition(partitions p);

void freePermutations(permutations p);

void freeLinkedList(struct Node *head);

#endif //EA_EQUIVALENCE_FREEMEMORY_H
