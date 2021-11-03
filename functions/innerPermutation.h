#ifndef EA_EQUIVALENCE_INNERPERMUTATION_H
#define EA_EQUIVALENCE_INNERPERMUTATION_H

#include "../utils/structs.h"

void innerPermutation(truthTable f, truthTable g, size_t *basis, size_t *l2, size_t *lPrime);

bool * computeSetOfTs(truthTable f, truthTable g, size_t x);

struct Node * computeDomain(const bool *listOfTs, truthTable f);

void reconstructInnerPermutation(struct Node **domains, size_t dimension, truthTable f, truthTable g, size_t *l2,
                                 size_t *lPrime);

bool dfs(struct Node **domains, size_t dimension, size_t k, size_t *values, truthTable f, truthTable g, size_t *l2,
         size_t *lPrime);

void reconstructTruthTable(const size_t *basisValues, size_t dimension, size_t *l2);

size_t * composeFunctions(const size_t *f, const size_t *g, size_t dimension);

size_t * addFunctionsTogether(const size_t *f, const size_t *g, size_t dimension);

bool isLinear(const size_t *f, size_t dimension);

#endif //EA_EQUIVALENCE_INNERPERMUTATION_H
