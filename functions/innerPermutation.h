#ifndef EA_EQUIVALENCE_INNERPERMUTATION_H
#define EA_EQUIVALENCE_INNERPERMUTATION_H

#include "../utils/structs.h"

void innerPermutation(truthTable f, truthTable g, size_t *basis);

bool * computeSetOfTs(truthTable f, truthTable g, size_t x);

struct Node computeDomain(const bool *listOfTs, truthTable f);

void reconstructInnerPermutation(size_t *domain, size_t *basis, size_t numOfBasis, size_t n);

void recursiveLoop(size_t *domain, size_t *basis, size_t numOfBasis, size_t n);

#endif //EA_EQUIVALENCE_INNERPERMUTATION_H
