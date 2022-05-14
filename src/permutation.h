#ifndef EA_EQUIVALENCE_PERMUTATION_H
#define EA_EQUIVALENCE_PERMUTATION_H

#include <stdio.h>
#include "structures.h"
#include "stdbool.h"

bool innerPermutation(TruthTable *f, TruthTable *g, const size_t *basis, TruthTable *l2, TruthTable **lPrime);

bool *computeSetOfTs(TruthTable *f, size_t x);

/**
 * Compute the restricted domain for the given list of T's.
 * The domain is represented with a linked list.
 * @param listOfTs A set of T's that we want to compute the domain for
 * @param f A function F
 * @return The restricted domain represented as a linked list.
 */
struct Node * computeDomain(const bool *listOfTs, TruthTable *f);

bool
dfs(Node **domains, size_t k, size_t *values, TruthTable *f, TruthTable *g, TruthTable *l2, TruthTable **lPrime);

/**
 * Reconstruction of a truth table
 * @param basisValues A standard basis values
 * @param dimension Working dimension
 */
void reconstructTruthTable(const size_t *basisValues, TruthTable *l2);

/**
 * Compose function F with function G
 * @param f Function that is f be composed
 * @param g Function that is composed with
 * @return The result of the composing
 */
TruthTable * composeFunctions(TruthTable *f, TruthTable *g);

/**
 * Add function F with function G
  * @param to Function to update
  * @param from Function to add with
  */
void addFunctionsTogether(TruthTable *to, TruthTable *from);

bool isLinear(TruthTable *f);

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

#endif //EA_EQUIVALENCE_PERMUTATION_H
