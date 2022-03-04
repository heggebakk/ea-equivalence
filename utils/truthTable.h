#ifndef EA_EQUIVALENCE_TRUTHTABLE_H
#define EA_EQUIVALENCE_TRUTHTABLE_H
#include "stdio.h"

typedef struct vbfTruthTable {
    size_t dimension;
    size_t *elements;
} truthTable;

void destroyTruthTable(truthTable *tt);

#endif //EA_EQUIVALENCE_TRUTHTABLE_H
