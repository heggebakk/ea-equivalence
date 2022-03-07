#ifndef EA_EQUIVALENCE_WALSHTRANSFORM_H
#define EA_EQUIVALENCE_WALSHTRANSFORM_H

#include "../utils/truthTable.h"
#include "../functions/partition.h"

typedef struct {
    size_t dimension;
    size_t **elements;
} WalshTransform;

void destroyWalshTransform(WalshTransform *wt);

_Bool dot(size_t a, size_t b);

size_t truthTableWalshTransform(TruthTable tt, size_t a, size_t b);

WalshTransform *truthTableToWalshTransform(TruthTable tt);

void printWalshTransformTable(WalshTransform wt);

size_t walshTransformPowerMoment(WalshTransform wt, size_t k, size_t shiftA, size_t shiftB);

partitions *eaPartitionWalsh(TruthTable *tt, size_t k);

#endif //EA_EQUIVALENCE_WALSHTRANSFORM_H
