#ifndef EA_EQUIVALENCE_PERMUTATION_H
#define EA_EQUIVALENCE_PERMUTATION_H

#include "structures.h"

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

#endif //EA_EQUIVALENCE_PERMUTATION_H
