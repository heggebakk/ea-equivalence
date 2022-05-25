#include "time.h"
#include <errno.h>
#include <stdio.h>
#include "stdlib.h"
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#include "permutation.h"
#include "structures.h"

/**
 * Reconstruction the inner permutations of function F and function G
 * @param F A TruthTable of function F
 * @param G A TruthTable of function G
 * @param basis
 * @param L2 The inner permutation result
 * @param L l'
 * @return Returns true if reconstruction of the inner permutation was successful, false otherwise.
 */
bool innerPermutation(TruthTable *F, TruthTable *G, const size_t *basis, TruthTable *L2, TruthTable **L) {
    struct Node **domain = calloc(sizeof(struct Node **), F->n); // The domain D(x) that will be reduced by computing the sets O(t)

    for (size_t i = 0; i < F->n; ++i) {
        bool *setOfT = computeSetOfTs(G, basis[i]); // Compute the set O(t)
        domain[i] = reduceDomain(setOfT, F); // Reduce the domain with the set O(t)
        free(setOfT);
    }
    size_t *values = malloc(sizeof(size_t) * F->n);
    bool result = dfs(domain, 0, values, F, G, L2, L);
    free(values);

    for (size_t i = 0; i < F->n; ++i) {
        destroyNodes(domain[i]);
    }
    free(domain);
    return result;
}

/**
 * Compute the sets O(t) of all triplets (x1,x2,x1+x2) with F(x1)+F(x2)+(Fx1+x2)=t
 * Is used to reduce the domains D(x)
 * @param F The function containing the elements to compute the sets over
 * @param x A fixed x to compute with
 * @return The set of t's from the computation
 */
bool *computeSetOfTs(TruthTable *F, size_t x) {
    size_t n = F->n;
    bool *set = calloc(sizeof(bool), 1L << n);

    for (size_t y = 0; y < 1L << n; ++y) {
        size_t t = F->elements[x] ^ F->elements[y] ^ F->elements[x ^ y];
        set[t] = true;
    }
    return set;
}

struct Node *reduceDomain(const bool *listOfTs, TruthTable *f) {
    size_t n = f->n;
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
    Node *head = initNode();
    for (size_t i = 0; i < 1L << n; ++i) {
        if (domain[i]) {
            // Add a new node to the linked list
            addToNode(head, i);
        }
    }
    free(domain);
    return head;
}

/**
 * L depth first search over a array containing linked lists.
 * @param domain L array containing a linked list of restricted domain
 * @param k
 * @param values
 * @param F The TruthTable of function F
 * @param G The TruthTable of function G
 * @param A2 The inner permutation that is to be computed
 * @param L The l' that is to be computed
 * @return True if the dfs successfully reconstructed a inner permutation, false otherwise.
 */

bool
dfs(Node **domain, size_t k, size_t *values, TruthTable *F, TruthTable *G, TruthTable *A2, TruthTable **L) {
    if (k >= F->n) {
        // L = F * A2 + G
        reconstructTruthTable(values, A2);
        *L = composeFunctions(F, A2);
        addFunctionsTogether(*L, G);
        if (isAffine(*L)) {
            return true;
        }
        destroyTruthTable(*L);
        return false;
    }
    Node *current = domain[k];
    while (current != NULL) {
        values[k] = current->data;
        bool linear = dfs(domain, k + 1, values, F, G, A2, L);
        if (linear) return true;
        current = (Node *) current->next;
    }
    return false;
}

void reconstructTruthTable(const size_t *basisValues, TruthTable *l2) {
    for (size_t coordinate = 0; coordinate < 1L << l2->n; ++coordinate) {
        size_t result = 0;
        for (size_t i = 0; i < l2->n; ++i) {
            if (1L << i & coordinate) {
                result ^= basisValues[i];
            }
        }
        l2->elements[coordinate] = result;
    }
}

TruthTable *composeFunctions(TruthTable *f, TruthTable *g) {
    TruthTable *result = initTruthTable(f->n);
    for (size_t x = 0; x < 1L << f->n; ++x) {
        result->elements[x] = f->elements[g->elements[x]];
    }
    return result;
}

void addFunctionsTogether(TruthTable *to, TruthTable *from) {
    for (size_t i = 0; i < 1L << to->n; ++i) {
        to->elements[i] ^= from->elements[i];
    }
}

/**
 * Check if a function is affine or not
 * @param f The function to check
 * @return True if the function is affine, false otherwise
 */
bool isAffine(TruthTable *f) {
    for (size_t a = 1; a < 1L << f->n; ++a) {
        for (size_t b = a + 1; b < 1L << f->n; ++b) {
            if (b > (a ^ b)) continue;
            size_t result = f->elements[0] ^ f->elements[a] ^ f->elements[b] ^ f->elements[a ^ b];
            if (result != 0) return false;
        }
    }
    return true;
}

TtNode *outerPermutation(Partition *f, Partition *g, size_t dimension, size_t *basis, size_t *fBucketPosition,
                         size_t *gBucketPosition, size_t *domainMap) {
    TtNode *l1 = initTtNode();
    size_t *images = calloc(sizeof(size_t), dimension); /* the images of the basis elements under L */
    size_t *generated = calloc(sizeof(size_t), 1L << dimension); /* a partial truth table for L */
    bool *generatedImages = calloc(sizeof(bool), 1L << dimension); /* a Boolean map showing which elements are among the images of the partially defined L */
    generatedImages[0] = true;

    /* Recursively guess the values of L on the basis (essentially, a DFS with backtracking upon contradiction) */
    recursive(0, basis, images, f, g, dimension, generated, generatedImages, l1, fBucketPosition, gBucketPosition,
              domainMap);

    free(images);
    free(generated);
    free(generatedImages);
    return l1;
}

void recursive(size_t k, const size_t *basis, size_t *images, Partition *partitionF, Partition *partitionG, size_t dimension,
               size_t *generated, bool *generatedImages, TtNode *l1, size_t *fBucketPosition, size_t *gBucketPosition,
               size_t *domainMap) {
    /* If all basis elements have been assigned an image, and no contradictions have occurred, then we have found
     * a linear permutation preserving the Partition. We reconstruct its truth-table, and add it to the linked
     * list containing all permutations found by the search.
     */
    if (k == dimension) {
        TruthTable *new = initTruthTable(dimension);
        memcpy(new->elements, generated, sizeof(size_t) * 1L << dimension);
        addTtNode(l1, new);
        destroyTruthTable(new);
        return;
    }

    /* We then take the bucket of the same size from the partition with
     * respect to G. We know that the image of the basis element must
     * belong to that bucket. */
    size_t posBg = domainMap[fBucketPosition[basis[k]]];

    /* We now go through all possible choice from the bucket */
    for (size_t ick = 0; ick < partitionG->bucketSizes[posBg]; ++ick) {
        size_t ck = partitionG->buckets[posBg][ick];
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
            if (partitionF->bucketSizes[fBucketPosition[x]] != partitionG->bucketSizes[gBucketPosition[y]]) {
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
                      fBucketPosition, gBucketPosition, domainMap);
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

bool hybridEquivalenceTest(Partition *f, Partition *g, size_t dimension, size_t *basis, size_t *fBucketPosition,
                           size_t *gBucketPosition, size_t *domainMap, TruthTable *functionF, TruthTable *functionG,
                           RunTimes *runTime) {
    size_t *images = calloc(sizeof(size_t), dimension); /* the images of the basis elements under L */
    size_t *generated = calloc(sizeof(size_t), 1L << dimension); /* a partial truth table for L */
    bool *generatedImages = calloc(sizeof(bool), 1L
            << dimension); /* a Boolean map showing which elements are among the images of the partially defined L */
    generatedImages[0] = true;

    /* Recursively guess the values of L on the basis (essentially, a DFS with backtracking upon contradiction) */

    bool foundSolution = false;
    size_t counter = 0;
    hybridRecursive(0, basis, images, f, g, dimension, generated, generatedImages, fBucketPosition, gBucketPosition,
                    domainMap, functionF, functionG, runTime, &foundSolution, &counter);
    printf("Counter: %zu\n", counter);

    free(images);
    free(generated);
    free(generatedImages);
    return foundSolution;
}

void hybridRecursive(size_t k, const size_t *basis, size_t *images, Partition *partitionF, Partition *partitionG,
                     size_t dimension, size_t *generated, bool *generatedImages, size_t *fBucketPosition,
                     size_t *gBucketPosition, size_t *domainMap, TruthTable *functionF, TruthTable *functionG,
                     RunTimes *runTime, bool *foundSolution, size_t *counter) {
    if (*foundSolution) return;

    /* If all basis elements have been assigned an image, and no contradictions have occurred, then we have found
     * a linear permutation preserving the Partition. We reconstruct its truth-table, and add it to the linked
     * list containing all permutations found by the search.
     */
    if (k == dimension) {
        TruthTable *L1 = initTruthTable(dimension);
        memcpy(L1->elements, generated, sizeof(size_t) * 1L << dimension);
//        addTtNode(l1, L1);
//        destroyTruthTable(L1);
        TruthTable *L1Inverse = inverse(L1);
        TruthTable *currentG = composeFunctions(L1Inverse, functionG);
        TruthTable *L;
        TruthTable *A2 = initTruthTable(dimension);
        clock_t startInnerPermutationTime = clock();
        bool foundInner = innerPermutation(functionF, currentG, basis, A2, &L);
        if (foundInner) {
            runTime->innerPermutation = stopTime(runTime->innerPermutation, startInnerPermutationTime);
            *foundSolution = true;
            // Find A, such that A = L1 * F * A2 + G
            TruthTable *A = composeFunctions(L1, L);

            // Print L1, A2 and A
            printf("L1:\n");
            printTruthTable(L1);
            printf("\nA2:\n");
            printTruthTable(A2);
            printf("\nA:\n");
            printTruthTable(A);
            printf("\n");

            // Free memory
            destroyTruthTable(A);
            destroyTruthTable(L1Inverse);
            destroyTruthTable(A2);
            destroyTruthTable(L);
            destroyTruthTable(currentG);
            return;
        }
        *counter += 1;
        destroyTruthTable(L1Inverse);
        destroyTruthTable(A2);
        destroyTruthTable(currentG);
        destroyTruthTable(L1);
        return;
    }

    /* We then take the bucket of the same size from the partition with
     * respect to G. We know that the image of the basis element must
     * belong to that bucket. */
    size_t posBg = domainMap[fBucketPosition[basis[k]]];

    /* We now go through all possible choice from the bucket */
    for (size_t ick = 0; ick < partitionG->bucketSizes[posBg]; ++ick) {
        size_t ck = partitionG->buckets[posBg][ick];
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
            if (partitionF->bucketSizes[fBucketPosition[x]] != partitionG->bucketSizes[gBucketPosition[y]]) {
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
            hybridRecursive(k + 1, basis, images, partitionF, partitionG, dimension, generated, generatedImages,
                            fBucketPosition, gBucketPosition, domainMap, functionF, functionG, runTime, foundSolution,
                            counter);
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

TruthTable *inverse(TruthTable *F) {
    TruthTable *result = initTruthTable(F->n);
    for (size_t x = 0; x < 1L << F->n; ++x) {
        size_t y = F->elements[x];
        result->elements[y] = x;
    }
    return result;
}