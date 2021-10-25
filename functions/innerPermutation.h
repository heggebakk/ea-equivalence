#ifndef EA_EQUIVALENCE_INNERPERMUTATION_H
#define EA_EQUIVALENCE_INNERPERMUTATION_H

#include "../utils/structs.h"

void innerPermutation(truthTable f, truthTable g, size_t *basis);

bool * computeSetOfTs(truthTable f, truthTable g, size_t x);

struct Node * computeDomain(const bool *listOfTs, truthTable f);

void reconstructInnerPermutation(struct Node **domains, size_t dimension);

void dfs(struct Node **domains, size_t dimension, size_t k, size_t *values);

void reconstructTruthTable(const size_t *basisValues, size_t dimension);

#endif //EA_EQUIVALENCE_INNERPERMUTATION_H
