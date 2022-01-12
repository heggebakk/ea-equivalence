#ifndef EA_EQUIVALENCE_NEWMAIN_H
#define EA_EQUIVALENCE_NEWMAIN_H

int newAlgorithm(truthTable *f, truthTable *g, size_t k, size_t dimension, size_t *basis);

int runWalshTransform(truthTable *f, truthTable *g, size_t k, size_t dimension, size_t *basis);

#endif //EA_EQUIVALENCE_NEWMAIN_H
