#ifndef EA_EQUIVALENCE_EATEST_H
#define EA_EQUIVALENCE_EATEST_H

#include "truthTable.h"

truthTable *randomLinearPermutation(size_t dimension);

truthTable *randomLinearFunction(size_t dimension);

truthTable * getFunctionG(truthTable *functionF);

#endif //EA_EQUIVALENCE_EATEST_H
