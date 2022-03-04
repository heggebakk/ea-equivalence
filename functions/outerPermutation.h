#ifndef EA_EQUIVALENCE_OUTERPERMUTATION_H
#define EA_EQUIVALENCE_OUTERPERMUTATION_H

#include <stdio.h>
#include "stdbool.h"
#include "partition.h"
#include "../utils/runTime.h"
#include "../utils/truthTable.h"
#include "../utils/linkedList.h"

size_t
outerPermutation(partitions *f, partitions *g, size_t dimension, size_t *basis, ttNode *l1, size_t *gClassPosition,
                 size_t *domainMap, FILE *fp, size_t *fClassPosition);

/**
 * Create a standard basis for the given dimension. 2^n where n = dimension.
 * @param dimension The dimension to create the basis from
 * @return The standard basis of 2^n, where n = dimension
 */
size_t *createStandardBasis(size_t dimension);

void recursive(size_t k, const size_t *basis, size_t *images, partitions *partitionF, partitions *partitionG, size_t n,
               size_t *generated, bool *generatedImages, ttNode *l1, size_t *fClassPosition, size_t *gClassPosition,
               size_t *domainMap);

#endif //EA_EQUIVALENCE_OUTERPERMUTATION_H
