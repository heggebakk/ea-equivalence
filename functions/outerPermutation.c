#include <stdbool.h>
#include "stdlib.h"
#include "outerPermutation.h"

permutations outerPermutation(partitions f, partitions g, size_t dimension, size_t *basis) {
    size_t n = dimension;
    basis = createBasis(n);
    size_t *images = calloc(sizeof(size_t), 1L << n);
    size_t *generated = calloc(sizeof(size_t), 1L << n);
    for (size_t i = 0; i < 1L << n; ++i) {
        generated[i] = 0;
    }
    bool generated_images[1L << n];
    for (size_t i = 0; i < 1L << n; ++i) {
        generated_images[i] = false;
    }
    generated_images[0] = true;
    permutations permutation;
    permutation.permutations = calloc(sizeof(size_t), 1L << n << n);
    permutation.numPermutations = 0;

    size_t *fBucketPosition = correspondingBucket(f, dimension);
    size_t *gBucketPosition = correspondingBucket(g, dimension);
    recursive(0, basis, images, f, g, n, generated, generated_images, &permutation, fBucketPosition, gBucketPosition);

    free(images);
    free(basis);
    free(generated);
    free(fBucketPosition);
    free(gBucketPosition);

    return permutation;
}

size_t *correspondingBucket(partitions function, size_t dimension) {
    size_t *list = (size_t *) calloc(sizeof(size_t), 1L << dimension);
    for (size_t i = 0; i < 1L << dimension; ++i) {
        for (size_t j = 0; j < function.numBuckets; ++j) {
            size_t *bucket = function.buckets[j];
            for (size_t k = 0; k < function.bucketSizes[j]; ++k) {
                if (bucket[k] == i) {
                    list[i] = j;
                    break;
                }
            }
        }
    }
    return list;
}

permutations *
recursive(size_t k, const size_t *basis, size_t *images, partitions partitionF, partitions partitionG, size_t n, size_t *generated,
          bool *generatedImages, permutations *permutation, const size_t *fBucketPosition, const size_t *gBucketPosition) {
    if (k == n) {
        size_t numPermutations = permutation->numPermutations;
        permutation->permutations[numPermutations] = calloc(sizeof(size_t), 1L << n);
        for (int i = 0; i < 1L << n; ++i) {
            permutation->permutations[numPermutations][i] = generated[i];
        }
        permutation->numPermutations += 1;
        return permutation;
    }

    size_t bf = partitionF.bucketSizes[fBucketPosition[basis[k]]];
    size_t posBg = findCorrespondingBucket(bf, partitionG);
    for (size_t ick = 0; ick < partitionG.bucketSizes[posBg]; ++ick) {
        size_t ck = partitionG.buckets[posBg][ick];
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
            if (partitionF.bucketSizes[fBucketPosition[x]] != partitionG.bucketSizes[gBucketPosition[y]]) {
                problem = true;
                break;
            }
            generated[x] = y;
            generatedImages[y] = true;
        }
        if (!problem) {
            images[k] = ck;
            recursive(k + 1, basis, images, partitionF, partitionG, n, generated, generatedImages, permutation,
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
    return permutation;
}

size_t findCorrespondingBucket(size_t bucketSizeF, partitions g) {
    for (size_t i = 0; i < g.numBuckets; ++i) {
        if (bucketSizeF == g.bucketSizes[i]) {
            return i;
        }
    }
    printf("Couldn't find a corresponding bucket with size %zu", bucketSizeF);
    exit(1);
}

bool isBijective(permutations outerPermutation, permutations innerPermutation) {
    if (outerPermutation.numPermutations <= 0 || innerPermutation.numPermutations <= 0) {
        printf("There are no permutations to check \n");
        return false;
    }
    bool isBijective = true;
    for (size_t i = 0; i < outerPermutation.numPermutations; ++i) {
        if (outerPermutation.permutations[i] != innerPermutation.permutations[i]) {
            isBijective = false;
            break;
        }
    }
    return !isBijective ? false : true;
}
