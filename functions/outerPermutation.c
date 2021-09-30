#include <stdbool.h>
#include "stdlib.h"
#include "outerPermutation.h"

size_t *basis;

permutations *outerPermutation(partitions f, partitions g, size_t dimension) {
    size_t n = dimension;
    basis = createBasis(n);
    size_t *images = malloc(sizeof(size_t) * 1L << n);
    size_t *generated = malloc(sizeof(size_t) * 1L << n); // TODO: What is generated?
    for (size_t i = 0; i < 1L << n; ++i) {
        generated[i] = 0;
    }
    bool generated_images[1L << n];
    for (size_t i = 0; i < 1L << n; ++i) {
        generated_images[i] = false;
    }
    generated_images[0] = true;
    permutations *permutation = malloc(sizeof(permutations *) * 1L << n << n);
    permutation->numPermutations = 0;
    size_t *fBucketPosition = correspondingBucket(f, dimension);
    size_t *gBucketPosition = correspondingBucket(g, dimension);
    recursive(0, images, f, g, n, generated, generated_images, permutation, fBucketPosition, gBucketPosition);

    free(images);
    free(basis);
    free(generated);
    free(fBucketPosition);
    free(gBucketPosition);

    return permutation;
}

size_t *createBasis(size_t dimension) {
    basis = malloc(sizeof(size_t) * dimension + 1);
    for (size_t i = 0; i < dimension; ++i) {
        basis[i] = 1L << i;
    }
    return basis;
}

size_t *correspondingBucket(partitions function, size_t dimension) {
    size_t *list = (size_t *) malloc(sizeof(size_t) * 1L << dimension);
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
recursive(size_t k, size_t *images, partitions partitionF, partitions partitionG, size_t n, size_t *generated,
          bool *generatedImages, permutations *permutation, size_t *fBucketPosition, size_t *gBucketPosition) {
    if (k == n) {
        permutation[permutation->numPermutations].permutations = malloc(sizeof(size_t) * 1L << n);
        for (int i = 0; i < 1L << n; ++i) {
            permutation[permutation->numPermutations].permutations[i] = generated[i];
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
            recursive(k + 1, images, partitionF, partitionG, n, generated, generatedImages, permutation,
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
    return NULL;
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

bool isBijective(permutations *p, size_t n) {
    if (p->numPermutations <= 0) {
        printf("There are no permutations to check \n");
        return false;
    }
    for (size_t pi = 0; pi < p->numPermutations - 1; ++pi) {
        for (size_t pj = pi + 1; pj < p->numPermutations; ++pj) {
            bool isBijective = false;
            for (size_t i = 0; i < 1L << n; ++i) {
                if (p[pi].permutations[i] != p[pj].permutations[i]) {
                    isBijective = true;
                    break;
                }
            }
            if (!isBijective) return false;
        }
    }
    return true;
}
