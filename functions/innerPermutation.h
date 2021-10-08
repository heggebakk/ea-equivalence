#ifndef EA_EQUIVALENCE_INNERPERMUTATION_H
#define EA_EQUIVALENCE_INNERPERMUTATION_H

#include "../utils/structs.h"

void innerPermutation(truthTable f, truthTable g);

bool * computeDomain(truthTable f, truthTable g, size_t x);

void computeSetOfTs(const bool *map, truthTable f);

#endif //EA_EQUIVALENCE_INNERPERMUTATION_H
