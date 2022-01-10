#ifndef EA_EQUIVALENCE_WALSHTRANSFORM_H
#define EA_EQUIVALENCE_WALSHTRANSFORM_H

#include "../utils/truthTable.h"
#include "partition.h"

typedef struct vbfWalshTransform {
    size_t dimension;
    size_t **elements;
} walshTransform;

_Bool dot(size_t a, size_t b);

size_t truthTableWalshTransform(truthTable tt, size_t a, size_t b);

walshTransform truthTableToWalshTransform(truthTable tt);

void printWalshTransformTable(walshTransform wt);

size_t walshTransformPowerMoment(walshTransform wt, size_t k, size_t shiftA, size_t shiftB);

partitions * eaPartitionWalsh(walshTransform wt, size_t k);

#endif //EA_EQUIVALENCE_WALSHTRANSFORM_H
