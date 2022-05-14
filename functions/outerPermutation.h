#ifndef EA_EQUIVALENCE_OUTERPERMUTATION_H
#define EA_EQUIVALENCE_OUTERPERMUTATION_H

#include <stdio.h>
#include "stdbool.h"
#include "../utils/runTime.h"
#include "../structures.h"

/**
 * Find all linear permutations L respecting two given Partition f and g, i.e. such that L maps all elements from a given
 * class in the partition under f to its corresponding class in the partition under g.
 * @param f The Partition for function F
 * @param g The Partition for function G
 * @param dimension The dimension
 * @param basis The basis
 * @param fClassPosition
 * @param gClassPosition
 * @param domainMap
 * @return The number of permutations found
 */
TtNode *outerPermutation(Partition *f, Partition *g, size_t dimension, size_t *basis, size_t *fClassPosition,
                         size_t *gClassPosition, size_t *domainMap);

/**
 * Create a standard basis for the given dimension. 2^n where n = dimension.
 * @param dimension The dimension to create the basis from
 * @return The standard basis of 2^n, where n = dimension
 */
size_t *createStandardBasis(size_t dimension);


/**
 * This is a DFS for linear permutations respecting the given Partition, i.e. such that every element from a given bucket
 * with respect to F maps to an element from a bucket with the same size corresponding to G.
 * @param k The recursive step
 * @param basis The basis of the dimension
 * @param images The
 * @param partitionF Partitions of function f
 * @param partitionG Partitions of function g
 * @param dimension The dimension
 * @param generated
 * @param generatedImages
 * @param l1
 * @param fClassPosition
 * @param gClassPosition
 * @param domainMap
 */
void recursive(size_t k, const size_t *basis, size_t *images, Partition *partitionF, Partition *partitionG, size_t dimension,
               size_t *generated, bool *generatedImages, TtNode *l1, size_t *fClassPosition, size_t *gClassPosition,
               size_t *domainMap);

#endif //EA_EQUIVALENCE_OUTERPERMUTATION_H
