#ifndef EA_EQUIVALENCE_NEWMAIN_H
#define EA_EQUIVALENCE_NEWMAIN_H

#include "utils/truthTable.h"

int newAlgorithm(truthTable *f, truthTable *g, size_t k, size_t dimension, size_t *basis, FILE *fp);

int runWalshTransform(truthTable *f, truthTable *g, size_t k, size_t dimension, size_t *basis, FILE *fp);

#endif //EA_EQUIVALENCE_NEWMAIN_H
