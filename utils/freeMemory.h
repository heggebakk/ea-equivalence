#ifndef EA_EQUIVALENCE_FREEMEMORY_H
#define EA_EQUIVALENCE_FREEMEMORY_H

#include "truthTable.h"
#include "linkedList.h"
#include "../functions/partition.h"
#include "../walshTransform/walshTransform.h"

void freeTruthTable(truthTable *tt);

void freeTtWt(walshTransform *wt);

void freePartition(partitions *p);

#endif //EA_EQUIVALENCE_FREEMEMORY_H
