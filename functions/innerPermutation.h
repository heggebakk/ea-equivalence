#ifndef EA_EQUIVALENCE_INNERPERMUTATION_H
#define EA_EQUIVALENCE_INNERPERMUTATION_H

#include "../utils/structs.h"

void innerPermutation(truthTable f, truthTable g);

bool * computeSetOfTs(truthTable f, truthTable g, size_t x);

size_t * computeDomain(const bool *listOfTs, truthTable f);

#endif //EA_EQUIVALENCE_INNERPERMUTATION_H
