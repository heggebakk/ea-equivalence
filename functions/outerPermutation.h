#ifndef EA_EQUIVALENCE_OUTERPERMUTATION_H
#define EA_EQUIVALENCE_OUTERPERMUTATION_H

#include "../utils/structs.h"
#include "../utils/linkedList.h"
#include "stdbool.h"

void outerPermutation(partitions f, partitions g, size_t dimension, size_t *basis, struct ttNode *l1);

size_t *createBasis(size_t dimension);

size_t *correspondingBucket(partitions function, size_t dimension);

void recursive(size_t k, const size_t *basis, size_t *images, partitions partitionF, partitions partitionG, size_t n,
               size_t *generated, bool *generatedImages, struct ttNode *l1, const size_t *fBucketPosition,
               const size_t *gBucketPosition);

//size_t *getBucket(size_t k, const size_t *bucketPositions, partitions function);

size_t findCorrespondingBucket(size_t bucketSizeF, partitions g);

bool isBijective(permutations outerPermutation, permutations innerPermutation);

#endif //EA_EQUIVALENCE_OUTERPERMUTATION_H
