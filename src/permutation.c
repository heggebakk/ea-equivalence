#include "stdlib.h"
#include <string.h>
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
bool innerPermutation(TruthTable *f, TruthTable *g, const size_t *basis, TruthTable *l2, TruthTable **lPrime) {
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
bool *computeSetOfTs(TruthTable *f, size_t x) {
    size_t n = f->dimension;
    bool *map = calloc(sizeof(bool), 1L << n);

    for (size_t y = 0; y < 1L << n; ++y) {
        size_t t = f->elements[x] ^ f->elements[y] ^ f->elements[x ^ y];
        map[t] = true;
    }
    return map;
}

struct Node *computeDomain(const bool *listOfTs, TruthTable *f) {
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
dfs(Node **domains, size_t k, size_t *values, TruthTable *f, TruthTable *g, TruthTable *l2, TruthTable **lPrime) {
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

void reconstructTruthTable(const size_t *basisValues, TruthTable *l2) {
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

TtNode *outerPermutation(Partition *f, Partition *g, size_t dimension, size_t *basis, size_t *fClassPosition,
                         size_t *gClassPosition, size_t *domainMap) {
    TtNode *l1 = initTtNode();
    size_t *images = calloc(sizeof(size_t), dimension); /* the images of the basis elements under L */
    size_t *generated = calloc(sizeof(size_t), 1L << dimension); /* a partial truth table for L */
    bool *generatedImages = calloc(sizeof(bool), 1L << dimension); /* a Boolean map showing which elements are among the images of the partially defined L */
    generatedImages[0] = true;

    /* Recursively guess the values of L on the basis (essentially, a DFS with backtracking upon contradiction) */
    recursive(0, basis, images, f, g, dimension, generated, generatedImages, l1, fClassPosition, gClassPosition,
              domainMap);

    free(images);
    free(generated);
    free(generatedImages);
    return l1;
}

size_t *createStandardBasis(size_t dimension) {
    size_t *basis = malloc(sizeof(size_t) * dimension + 1);
    for (size_t i = 0; i < dimension; ++i) {
        basis[i] = 1L << i;
    }
    return basis;
}

void recursive(size_t k, const size_t *basis, size_t *images, Partition *partitionF, Partition *partitionG, size_t dimension,
               size_t *generated, bool *generatedImages, TtNode *l1, size_t *fClassPosition, size_t *gClassPosition,
               size_t *domainMap) {
    /* If all basis elements have been assigned an image, and no contradictions have occurred, then we have found
     * a linear permutation preserving the Partition. We reconstruct its truth-table, and add it to the linked
     * list containing all permutations found by the search.
     */
    if (k == dimension) {
        TruthTable *new = initTruthTable(dimension);
        memcpy(new->elements, generated, sizeof(size_t) * 1L << dimension);
        addNode(l1, new);
        destroyTruthTable(new);
        return;
    }

    /* We then take the bucket of the same size from the partition with
     * respect to G. We know that the image of the basis element must
     * belong to that bucket.
     */
    size_t posBg = domainMap[fClassPosition[basis[k]]];
    /* We now go through all possible choice from the bucket */
    for (size_t ick = 0; ick < partitionG->classSizes[posBg]; ++ick) {
        size_t ck = partitionG->classes[posBg][ick];
        /* Since we want the function to be a permutation, the image of the basis element
         * should not be one of the images that we have already generated.
         */
        if (generatedImages[ck]) continue;
        /* A contradiction can occur if assigning this value to the basis element causes some other
         * element to map to the wrong bucket by linearity. The following variable will be set to
         * true if such a contradiction is encountered.
         */
        bool problem = false;

        /* This is done to handle the special case of k = 0, since otherwise we get (1L << (k-1)) == MAX_INT. */
        size_t LIMIT = k ? 1L << k : 1;

        /* We now go through all linear combinations of the basis elements that have been previously assigned.
         * Adding the newly guessed basis element to such combination allows us to derive one more value of the
         * function; if one of these values maps to the wrong bucket, we set "problem" = false, to indicate a
         * contradiction and backtrack.
         */
        for (size_t linearCombination = 0; linearCombination < LIMIT; ++linearCombination) {
            size_t x = linearCombination ^ basis[k];
            size_t y = ck;
            /* The following loop simply XOR's all images corresponding to the linear combination, so that
             * we get its image by linearity.
             */
            if (k) {
                for (size_t i = 0; i < k; ++i) {
                    if (1L << i & linearCombination) {
                        y ^= generated[basis[i]];
                    }
                }
            }

            /* Check for contradiction as described above. */
            if (partitionF->classSizes[fClassPosition[x]] != partitionG->classSizes[gClassPosition[y]]) {
                problem = true;
                break;
            }
            /* Add the new preimage-image pair to the partial truth table of the function */
            generated[x] = y;
            /* We also indicate that the image belongs to the set of generated images */
            generatedImages[y] = true;
        }
        /* If no contradiction is encountered, we go to the next basis element. */
        if (!problem) {
            images[k] = ck;
            recursive(k + 1, basis, images, partitionF, partitionG, dimension, generated, generatedImages, l1,
                      fClassPosition, gClassPosition, domainMap);
        }

        /* When backtracking, we need to reset the generated image indicators */
        for (size_t linearCombinations = 0; linearCombinations < LIMIT; ++linearCombinations) {
            size_t y = ck;
            if (k) {
                for (size_t i = 0; i < k; ++i) {
                    if (1L << i & linearCombinations) {
                        y ^= generated[basis[i]];
                    }
                }
            }
            generatedImages[y] = false;
        }
    }
}