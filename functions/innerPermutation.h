#ifndef EA_EQUIVALENCE_INNERPERMUTATION_H
#define EA_EQUIVALENCE_INNERPERMUTATION_H

#include "../utils/truthTable.h"
#include "stdbool.h"
#include "../utils/linkedList.h"

/**
 * Reconstruction the inner permutations of function F and function G
 * @param f A TruthTable of function F
 * @param g A TruthTable of function G
 * @param basis
 * @param l2 The inner permutation result
 * @param lPrime l'
 * @return Returns true if reconstruction of the inner permutation was successful, false otherwise.
 */
bool innerPermutation(TruthTable *f, TruthTable *g, const size_t *basis, TruthTable *l2, TruthTable **lPrime);

/**
 * Compute the set of t's where t = f[x] + f[y] + f[x + y]
 * @param f The function containing the elements to compute the sets over
 * @param x A fixed x to compute with
 * @return The set of t's from the computation
 */
bool *computeSetOfTs(TruthTable *f, size_t x);

/**
 * Compute the restricted domain for the given list of T's.
 * The domain is represented with a linked list.
 * @param listOfTs A set of T's that we want to compute the domain for
 * @param f A function F
 * @return The restricted domain represented as a linked list.
 */
Node * computeDomain(const bool *listOfTs, TruthTable *f);

/**
 * A depth first search over a array containing linked lists.
 * @param domains A array containing a linked list of restricted domains
 * @param k
 * @param values
 * @param f The TruthTable of function f
 * @param g The TruthTable of function g
 * @param l2 The inner permutation that is to be computed
 * @param lPrime The l' that is to be computed
 * @return True if the dfs successfully reconstructed a inner permutation, false otherwise.
 */
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

/**
 * Check if a function is linear or not
 * @param f The function to check
 * @return True if the function is linear, false otherwise
 */
bool isLinear(TruthTable *f);

#endif //EA_EQUIVALENCE_INNERPERMUTATION_H
