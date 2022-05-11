#include <stdbool.h>
#include <string.h>
#include "stdlib.h"
#include "outerPermutation.h"
#include "partition.h"

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
        if (generatedImages[ck] == true) {
            continue;
        }
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
