#ifndef EA_EQUIVALENCE_EATEST_H
#define EA_EQUIVALENCE_EATEST_H

#include "truthTable.h"

TruthTable *randomLinearPermutation(size_t dimension);

TruthTable *randomLinearFunction(size_t dimension);

TruthTable * getFunctionG(TruthTable *functionF);

#endif //EA_EQUIVALENCE_EATEST_H
