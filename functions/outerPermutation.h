#ifndef EA_EQUIVALENCE_OUTERPERMUTATION_H
#define EA_EQUIVALENCE_OUTERPERMUTATION_H

#include "../utils/truthTable.h"
#include "../utils/linkedList.h"
#include "stdbool.h"
#include "partition.h"

void outerPermutation(partitions *f, partitions *g, size_t dimension, size_t *basis, struct ttNode *l1);

/**
 * Create a standard basis for the given dimension. 2^n where n = dimension.
 * @param dimension The dimension to create the basis from
 * @return The standard basis of 2^n, where n = dimension
 */
size_t *createBasis(size_t dimension);

size_t *correspondingPermutationClass(partitions *partition, size_t dimension);

void recursive(size_t k, const size_t *basis, size_t *images, partitions *partitionF, partitions *partitionG, size_t n,
               size_t *generated, bool *generatedImages, struct ttNode *l1, const size_t *fClassPosition,
               const size_t *gClassPosition);

//size_t *getBucket(size_t k, const size_t *bucketPositions, partitions function);

size_t findCorrespondingClass(size_t classSizeF, partitions *g);

bool isBijective(truthTable *outerPermutation, truthTable *innerPermutation);

size_t findOuterPermutation(size_t DIMENSION, partitions *partitionF, partitions *partitionG, size_t *basis,
                            struct ttNode *l1, FILE *fp);

#endif //EA_EQUIVALENCE_OUTERPERMUTATION_H
