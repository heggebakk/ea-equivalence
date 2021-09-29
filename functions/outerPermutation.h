#ifndef EA_EQUIVALENCE_OUTERPERMUTATION_H
#define EA_EQUIVALENCE_OUTERPERMUTATION_H

#include "../utils/structs.h"

permutations *outerPermutation(partitions f, partitions g);

size_t *createBasis(size_t dimension);

size_t *correspondingBucket(partitions function);

permutations *
recursive(size_t k, size_t *images, partitions partitionF, partitions partitionG, size_t n, size_t *generated,
          bool *generatedImages, permutations *permutation, size_t *fBucketPosition, size_t *gBucketPosition);

bucket *getBucket(size_t k, const size_t *bucketPositions, partitions function);

bucket *findCorrespondingBucket(bucket b, partitions function);

bool isBijective(permutations *p, size_t n);

#endif //EA_EQUIVALENCE_OUTERPERMUTATION_H
