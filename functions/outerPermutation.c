#include <stdbool.h>
#include <string.h>
#include "stdlib.h"
#include "outerPermutation.h"

void outerPermutation(partitions *f, partitions *g, size_t dimension, size_t *basis, struct ttNode *l1) {
    basis = createBasis(dimension);
    size_t *images = calloc(sizeof(size_t), 1L << dimension);
    size_t *generated = calloc(sizeof(size_t), 1L << dimension);
    for (size_t i = 0; i < 1L << dimension; ++i) {
        generated[i] = 0;
    }
    bool generated_images[1L << dimension];
    for (size_t i = 0; i < 1L << dimension; ++i) {
        generated_images[i] = false;
    }
    generated_images[0] = true;

    size_t *fBucketPosition = correspondingBucket(f, dimension);
    size_t *gBucketPosition = correspondingBucket(g, dimension);
    recursive(0, basis, images, f, g, dimension, generated, generated_images, l1, fBucketPosition, gBucketPosition);

    free(images);
    free(basis);
    free(generated);
    free(fBucketPosition);
    free(gBucketPosition);
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
size_t *correspondingBucket(partitions *partition, size_t dimension) {
    size_t *list = (size_t *) calloc(sizeof(size_t), 1L << dimension);

    for (size_t i = 0; i < 1L << dimension; ++i) {
        for (size_t j = 0; j < partition->numBuckets; ++j) {
            size_t *bucket = partition->buckets[j];
            for (size_t k = 0; k < partition->bucketSizes[j]; ++k) {
                if (bucket[k] == i) {
                    list[i] = j;
                    break;
                }
            }
        }
    }
    return list;
}

void recursive(size_t k, const size_t *basis, size_t *images, partitions *partitionF, partitions *partitionG, size_t n,
               size_t *generated, bool *generatedImages, struct ttNode *l1, const size_t *fBucketPosition,
               const size_t *gBucketPosition) {
    if (k == n) {
        truthTable * new = malloc(sizeof(truthTable));
        new->dimension = n;
        new->elements = calloc(sizeof(size_t), 1L << n);
        memcpy(new->elements, generated, sizeof(size_t) * 1L << n);
        addNode(l1, new);
        return;
    }

    size_t bf = partitionF->bucketSizes[fBucketPosition[basis[k]]];
    size_t posBg = findCorrespondingBucket(bf, partitionG);
    for (size_t ick = 0; ick < partitionG->bucketSizes[posBg]; ++ick) {
        size_t ck = partitionG->buckets[posBg][ick];
        if (generatedImages[ck] == true) {
            continue;
        }
        bool problem = false;

        /* a very poor but effective way to handle the case when k = 0, because otherwise for k = 0 we get
         * (1L << (k-1)) == MAX_INT, and it loops figuratively forever */
        size_t LIMIT = k ? 1L << k : 1;

        for (size_t linearCombinations = 0; linearCombinations < LIMIT; ++linearCombinations) {
            size_t x = linearCombinations ^ basis[k];
            size_t y = ck;
            if (k) {
                for (size_t i = 0; i < k; ++i) {
                    if (1L << i & linearCombinations) {
                        y ^= generated[basis[i]];
                    }
                }
            }
            if (partitionF->bucketSizes[fBucketPosition[x]] != partitionG->bucketSizes[gBucketPosition[y]]) {
                problem = true;
                break;
            }
            generated[x] = y;
            generatedImages[y] = true;
        }
        if (!problem) {
            images[k] = ck;
            recursive(k + 1, basis, images, partitionF, partitionG, n, generated, generatedImages, l1,
                      fBucketPosition, gBucketPosition);
        }

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

size_t findCorrespondingBucket(size_t bucketSizeF, partitions *g) {
    for (size_t i = 0; i < g->numBuckets; ++i) {
        if (bucketSizeF == g->bucketSizes[i]) {
            return i;
        }
    }
    printf("Couldn't find a corresponding bucket with size %zu\n", bucketSizeF);
    exit(1);
}

bool isBijective(truthTable *outerPermutation, truthTable *innerPermutation) {
    if (outerPermutation->elements == NULL || innerPermutation->elements == NULL) {
        printf("There are no permutations to check \n");
        return false;
    }

    bool isBijective = true;
    for (size_t i = 0; i < 1L << outerPermutation->dimension; ++i) {
        if (outerPermutation->elements[i] != innerPermutation->elements[i]) {
            return false;
        }
    }
    return isBijective;
}
