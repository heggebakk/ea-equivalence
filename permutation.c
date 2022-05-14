#include <stdbool.h>
#include <malloc.h>
#include "permutation.h"
#include "structures.h"

/**
 * Reconstruction the inner permutations of function F and function G
 * @param f A TruthTable of function F
 * @param g A TruthTable of function G
 * @param basis
 * @param l2 The inner permutation result
 * @param lPrime l'
 * @return Returns true if reconstruction of the inner permutation was successful, false otherwise.
 */
bool innerPermutation(struct TruthTable *f, struct TruthTable *g, const size_t *basis, struct TruthTable *l2, struct TruthTable **lPrime) {
    struct Node **restrictedDomains = calloc(sizeof(struct Node **), f->dimension); // A list of Linked Lists

    for (size_t i = 0; i < f->dimension; ++i) {
        bool *map = computeSetOfTs(g, basis[i]);
        restrictedDomains[i] = computeDomain(map, f);
        free(map);
    }
    size_t *values = malloc(sizeof(size_t) * f->dimension);
    bool result = dfs(restrictedDomains, 0, values, f, g, l2, lPrime);
    free(values);

    for (size_t i = 0; i < f->dimension; ++i) {
        destroyLinkedList(restrictedDomains[i]);
    }
    free(restrictedDomains);
    return result;
}

/**
 * Compute the set of t's where t = f[x] + f[y] + f[x + y]
 * @param f The function containing the elements to compute the sets over
 * @param x A fixed x to compute with
 * @return The set of t's from the computation
 */
bool *computeSetOfTs(struct TruthTable *f, size_t x) {
    size_t n = f->dimension;
    bool *map = calloc(sizeof(bool), 1L << n);

    for (size_t y = 0; y < 1L << n; ++y) {
        size_t t = f->elements[x] ^ f->elements[y] ^ f->elements[x ^ y];
        map[t] = true;
    }
    return map;
}

struct Node *computeDomain(const bool *listOfTs, struct TruthTable *f) {
    size_t n = f->dimension;
    bool *domain = calloc(sizeof(bool), 1L << n);
    for (size_t i = 0; i < 1L << n; ++i) {
        domain[i] = true;
    }
    for (size_t t = 0; t < 1L << n; ++t) {
        if (listOfTs[t]) {
            bool *tempSet = calloc(sizeof(bool), 1L << n);
            for (size_t x = 0; x < 1L << n; ++x) {
                for (size_t y = 0; y < 1L << n; ++y) {
                    if (t == (f->elements[x] ^ f->elements[y] ^ f->elements[x ^ y])) {
                        tempSet[x] = true;
                        tempSet[y] = true;
                        tempSet[x ^ y] = true;
                    }
                }
            }
            for (size_t i = 0; i < 1L << n; ++i) {
                domain[i] &= tempSet[i];
            }
            free(tempSet);
        }
    }

    // Restricted domain represented as a Linked List
    Node *head = initLinkedList();
    for (size_t i = 0; i < 1L << n; ++i) {
        if (domain[i]) {
            // Add a new node to the linked list
            addToLinkedList(head, i);
        }
    }
    free(domain);
    return head;
}

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
dfs(Node **domains, size_t k, size_t *values, struct TruthTable *f, struct TruthTable *g, struct TruthTable *l2, struct TruthTable **lPrime) {
    if (k >= f->dimension) {
        reconstructTruthTable(values, l2);
        *lPrime = composeFunctions(f, l2);
        addFunctionsTogether(*lPrime, g);
        if (isLinear(*lPrime)) {
            return true;
        }
        destroyTruthTable(*lPrime);
        return false;
    }
    Node *current = domains[k];
    while (current != NULL) {
        values[k] = current->data;
        bool linear = dfs(domains, k + 1, values, f, g, l2, lPrime);
        if (linear) return true;
        current = (Node *) current->next;
    }
    return false;
}

void reconstructTruthTable(const size_t *basisValues, struct TruthTable *l2) {
    for (size_t coordinate = 0; coordinate < 1L << l2->dimension; ++coordinate) {
        size_t result = 0;
        for (size_t i = 0; i < l2->dimension; ++i) {
            if (1L << i & coordinate) {
                result ^= basisValues[i];
            }
        }
        l2->elements[coordinate] = result;
    }
}

TruthTable *composeFunctions(TruthTable *f, TruthTable *g) {
    TruthTable *result = initTruthTable(f->dimension);
    for (size_t x = 0; x < 1L << f->dimension; ++x) {
        result->elements[x] = f->elements[g->elements[x]];
    }
    return result;
}

void addFunctionsTogether(TruthTable *to, TruthTable *from) {
    for (size_t i = 0; i < 1L << to->dimension; ++i) {
        to->elements[i] ^= from->elements[i];
    }
}

/**
 * Check if a function is linear or not
 * @param f The function to check
 * @return True if the function is linear, false otherwise
 */
bool isLinear(TruthTable *f) {
    for (size_t a = 1; a < 1L << f->dimension; ++a) {
        for (size_t b = a + 1; b < 1L << f->dimension; ++b) {
            if (b > (a ^ b)) continue;
            size_t result = f->elements[0] ^ f->elements[a] ^ f->elements[b] ^ f->elements[a ^ b];
            if (result != 0) return false;
        }
    }
    return true;
}