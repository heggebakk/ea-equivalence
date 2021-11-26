#ifndef EA_EQUIVALENCE_INNERPERMUTATION_H
#define EA_EQUIVALENCE_INNERPERMUTATION_H

#include "../utils/structs.h"
#include "stdbool.h"

bool innerPermutation(truthTable *f, truthTable *g, const size_t *basis, truthTable *l2, truthTable **lPrime);

bool * computeSetOfTs(truthTable *f, truthTable *g, size_t x);

struct Node * computeDomain(const bool *listOfTs, truthTable *f);

bool
dfs(struct Node **domains, size_t dimension, size_t k, size_t *values, truthTable *f, truthTable *g, truthTable *l2,
    truthTable **lPrime);

void reconstructTruthTable(const size_t *basisValues, truthTable *l2);

truthTable * composeFunctions(truthTable *f, truthTable *g);

void addFunctionsTogether(truthTable *to, truthTable *from);

bool isLinear(truthTable *f);

#endif //EA_EQUIVALENCE_INNERPERMUTATION_H
