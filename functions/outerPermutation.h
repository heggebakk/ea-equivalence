#ifndef EA_EQUIVALENCE_OUTERPERMUTATION_H
#define EA_EQUIVALENCE_OUTERPERMUTATION_H

#include "../utils/structs.h"

size_t outerPermutation(partitions f, partitions g, size_t dimension, size_t *basis, truthTable **outerPerm);

size_t *createBasis(size_t dimension);

size_t *correspondingBucket(partitions function, size_t dimension);

size_t recursive(size_t k, const size_t *basis, size_t *images, partitions partitionF, partitions partitionG, size_t n,
                 size_t *generated, bool *generatedImages, truthTable **permutations, const size_t *fBucketPosition,
                 const size_t *gBucketPosition, size_t *numPermutations);

//size_t *getBucket(size_t k, const size_t *bucketPositions, partitions function);

size_t findCorrespondingBucket(size_t bucketSizeF, partitions g);

bool isBijective(permutations outerPermutation, permutations innerPermutation);

#endif //EA_EQUIVALENCE_OUTERPERMUTATION_H
