#ifndef EA_EQUIVALENCE_PERMUTATION_H
#define EA_EQUIVALENCE_PERMUTATION_H

#include <stdio.h>
#include "structures.h"
#include "stdbool.h"

bool innerPermutation(TruthTable *F, TruthTable *G, const size_t *basis, TruthTable *L2, TruthTable **L);

bool *computeSetOfTs(TruthTable *F, size_t x);

/**
 * Calculate the inverse of a function F
 * @param F The function F to inverse
 * @return The inverse of function F
 */
TruthTable *inverse(TruthTable *F);

/**
 * Compute the restricted domain for the given list of T's.
 * The domain is represented with a linked list.
 * @param listOfTs A set of T's that we want to compute the domain for
 * @param f A function F
 * @return The restricted domain represented as a linked list.
 */
struct Node * reduceDomain(const bool *listOfTs, TruthTable *f);

bool
dfs(Node **domain, size_t k, size_t *values, TruthTable *F, TruthTable *G, TruthTable *A2, TruthTable **L);

/**
 * Reconstruction of a truth table
 * @param basisValues A standard basis values
 * @param dimension Working n
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

bool isAffine(TruthTable *f);

/**
 * Find all linear permutations L respecting two given Partition f and g, i.e. such that L maps all elements from a given
 * class in the partition under f to its corresponding class in the partition under g.
 * @param f The Partition for function F
 * @param g The Partition for function G
 * @param dimension The n
 * @param basis The basis
 * @param fBucketPosition
 * @param gBucketPosition
 * @param domainMap
 * @return The number of permutations found
 */
TtNode *outerPermutation(Partition *f, Partition *g, size_t dimension, size_t *basis, size_t *fBucketPosition,
                         size_t *gBucketPosition, size_t *domainMap);

/**
 * This is a DFS for linear permutations respecting the given Partition, i.e. such that every element from a given bucket
 * with respect to F maps to an element from a bucket with the same size corresponding to G.
 * @param k The recursive step
 * @param basis The basis of the n
 * @param images The
 * @param partitionF Partitions of function f
 * @param partitionG Partitions of function g
 * @param dimension The n
 * @param generated
 * @param generatedImages
 * @param l1
 * @param fBucketPosition
 * @param gBucketPosition
 * @param domainMap
 */
void recursive(size_t k, const size_t *basis, size_t *images, Partition *partitionF, Partition *partitionG, size_t dimension,
               size_t *generated, bool *generatedImages, TtNode *l1, size_t *fBucketPosition, size_t *gBucketPosition,
               size_t *domainMap);

bool hybridEquivalenceTest(Partition *f, Partition *g, size_t dimension, size_t *basis, size_t *fBucketPosition,
                           size_t *gBucketPosition, size_t *domainMap, TruthTable *functionF, TruthTable *functionG,
                           RunTimes *runTime);

void hybridRecursive(size_t k, const size_t *basis, size_t *images, Partition *partitionF, Partition *partitionG,
                     size_t dimension, size_t *generated, bool *generatedImages, size_t *fBucketPosition,
                     size_t *gBucketPosition, size_t *domainMap, TruthTable *functionF, TruthTable *functionG,
                     RunTimes *runTime, bool *foundSolution, size_t *counter);

#endif //EA_EQUIVALENCE_PERMUTATION_H
