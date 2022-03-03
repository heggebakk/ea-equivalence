#include <stdbool.h>
#include <string.h>
#include "stdlib.h"
#include "outerPermutation.h"
#include "partition.h"

/* Find all linear permutations L respecting two given partitions f and g, i.e. such that L maps all elements from a given
 * class in the partition under f to its corresponding class in the partition under g.
 */
void
outerPermutation(partitions *f, partitions *g, size_t dimension, size_t *basis, ttNode *l1, size_t *partitionMap,
                 size_t *domainMap) {
    basis = createBasis(dimension); /* We will guess the values of L on a linear basis */
    size_t *images = calloc(sizeof(size_t), dimension); /* the images of the basis elements under L */
    size_t *generated = calloc(sizeof(size_t), 1L << dimension); /* a partial truth table for L */
    for (size_t i = 0; i < 1L << dimension; ++i) {
        generated[i] = 0;
    }
    bool generated_images[1L << dimension]; /* a Boolean map showing which elements are among the images of the partially defined L */
    for (size_t i = 0; i < 1L << dimension; ++i) {
        generated_images[i] = false;
    }
    generated_images[0] = true;

    /* Create dictionaries indexing buckets by elements (for instance, fClassPosition[i] would be
     * the index of the bucket w.r.t. f containing the element i)
     */
    size_t *fClassPosition = correspondingPermutationClass(f, dimension);
    size_t *gClassPosition = correspondingPermutationClass(g, dimension);

    /* Recursively guess the values of L on the basis (essentially, a DFS with backtracking upon contradiction) */
    recursive(0, basis, images, f, g, dimension, generated, generated_images, l1, fClassPosition, gClassPosition,
              partitionMap, domainMap);

    free(images);
    free(basis);
    free(generated);
    free(fClassPosition);
    free(gClassPosition);
}

size_t *createBasis(size_t dimension) {
    size_t *basis = malloc(sizeof(size_t) * dimension + 1);
    for (size_t i = 0; i < dimension; ++i) {
        basis[i] = 1L << i;
    }
    return basis;
}

/**
 * // TODO: Add description
 * @param partition
 * @param dimension
 * @return
 */
size_t *correspondingPermutationClass(partitions *partition, size_t dimension) {
    size_t *correspondingClass = (size_t *) calloc(sizeof(size_t), 1L << dimension);

    for (size_t i = 0; i < 1L << dimension; ++i) {
        for (size_t j = 0; j < partition->numberOfClasses; ++j) {
            size_t *partitionClass = partition->classes[j];
            for (size_t k = 0; k < partition->classSizes[j]; ++k) {
                if (partitionClass[k] == i) {
                    correspondingClass[i] = j;
                    break;
                }
            }
        }
    }
    printf("Corresponding class: \n");
    for (int i = 0; i < 1L << dimension; ++i) {
        printf("%zu ", correspondingClass[i]);
    }
    printf("\n");
    printPartitionInfo(partition);
    return correspondingClass;
}

/* This is a DFS for linear permutations respecting the given partitions, i.e. such that every element from a given bucket
 * with respect to F maps to an element from a bucket with the same size corresponding to G.
 */
void recursive(size_t k, const size_t *basis, size_t *images, partitions *partitionF, partitions *partitionG, size_t n,
               size_t *generated, bool *generatedImages, struct ttNode *l1, const size_t *fClassPosition,
               const size_t *gClassPosition, size_t *partitionMap, size_t *domainMap) {
    /* If all basis elements have been assigned an image, and no contradictions have occurred, then we have found
     * a linear permutation preserving the partitions. We reconstruct its truth-table, and add it to the linked
     * list containing all permutations found by the search.
     */
    if (k == n) {
        truthTable * new = malloc(sizeof(truthTable));
        new->dimension = n;
        new->elements = calloc(sizeof(size_t), 1L << n);
        memcpy(new->elements, generated, sizeof(size_t) * 1L << n);
        addNode(l1, new);
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
	 * function; if one of these values maps to the wrong bucket, we set "problem" to false to indicate a
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
            recursive(k + 1, basis, images, partitionF, partitionG, n, generated, generatedImages, l1,
                      fClassPosition, gClassPosition, partitionMap, domainMap);
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

size_t
findOuterPermutation(size_t DIMENSION, partitions *partitionF, partitions *partitionG, size_t *basis, ttNode *l1,
                     FILE *fp, size_t *partitionMap, size_t *domainMap) {
    outerPermutation(partitionF, partitionG, DIMENSION, basis, l1, partitionMap, domainMap);
    printf("\n");
    size_t numPerm = countTtNodes(l1);
    fprintf(fp, "// Number of permutations:\n%zu \n", numPerm);
    return numPerm;
}
