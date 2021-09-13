#include <stdlib.h>
#include <stdbool.h>
#include "permutation.h"
#include "time.h"

size_t *basis;

permutations * outerPermutation(partitions f, partitions g) {
    size_t n = f.dimension;
    basis = createBasis(n);

    size_t *images = malloc(sizeof(size_t) * (1L << n));
    size_t *generated = malloc(sizeof(size_t) *  (1L << n));
    // Assign each position in Generated with 0
    for (size_t i = 0; i < 1L << n; ++i) {
        generated[i] = 0;
    }
    _Bool generated_images [(1L << n)];
    for(size_t i = 0; i < (1L << n); ++i) {
        generated_images[i] = false;
    }
    generated_images[0] = true;
    permutations *permutation;
    permutation = malloc(sizeof(permutations *) * 1L << n << n);
    permutation->numPermutations = 0;
    size_t *fBucketPosition = correspondingBucket(f); // A list representing in which bucket each k is
    size_t *gBucketPosition = correspondingBucket(g);
    recursive(0, images, f, g, n, generated, generated_images, permutation, fBucketPosition, gBucketPosition);

    free(images);
    free(basis);
    free(generated);
    free(fBucketPosition);
    free(gBucketPosition);

    return permutation;
}

bool isBijective(permutations *permutation, size_t n) {
    if (permutation->numPermutations <= 0) {
        printf("There are no permutations to check\n");
        return false;
    }
    for (size_t pi = 0; pi < permutation->numPermutations - 1; ++pi) {
        for (size_t pj = pi + 1; pj < permutation->numPermutations; ++pj) {
            bool bijective = false;
            for (size_t i = 0; i < 1L << n; ++i) {
                if (permutation[pi].permutations[i] != permutation[pj].permutations[i]) {
                    bijective = true;
                    break;
                }
            }
            if (!bijective) return false;
        }
    }
    return true;
}

bool isLinear(permutations *permutation, size_t k, size_t n) {
    //TODO
//    if (k == n) {
//
//    }
    return false;
}

bucket *findCorrespondingBucket(bucket bucketB, partitions function) {
    for (size_t i = 0; i < function.numBuckets; ++i) {
        bucket *currentBucket = &(*function.buckets[i]);
        if (bucketB.bucketSize == currentBucket->bucketSize) {
            return currentBucket;
        }
    }
    printf("Couldn't find a corresponding bucket to bucket with bucketSize %zu", bucketB.bucketSize);
    return NULL;
}

size_t * correspondingBucket(partitions function) {
    size_t *list = (size_t *) malloc(sizeof(size_t) * 1L << function.dimension);

    for (size_t i = 0; i < 1L << function.dimension; ++i) {
        for (int j = 0; j < function.numBuckets; ++j) {
            bucket *bucket = function.buckets[j];
            for (size_t k = 0; k < bucket->bucketSize; ++k) {
                if (bucket->elements[k] == i) {
                    list[i] = j;
                    break;
                }
            }
        }
    }
    return list;
}

bucket *getBucket(size_t k, const size_t *bucketPositions, partitions function) {
    return function.buckets[bucketPositions[k]];
}

size_t *createBasis(size_t dimension) {
    basis = malloc(sizeof(size_t) * dimension + 1);
    for (size_t i = 0; i < dimension; ++i) {
        basis[i] = 1L << i;
    }
    return basis;
}

permutations
*recursive(size_t k, size_t *images, partitions partitionF, partitions partitionG, size_t n, size_t *generated,
           bool *generated_images, permutations *permutation, size_t *fBucketPosition, size_t *gBucketPosition) {
    if (k == n) {
        permutation[permutation->numPermutations].permutations = malloc(sizeof(size_t) * 1L << n);
        for (size_t i = 0; i < (1L << n); ++i) {
            permutation[permutation->numPermutations].permutations[i] = generated[i];
        }
        permutation->numPermutations += 1;
        return permutation;
    }

    bucket *bf = getBucket(basis[k], fBucketPosition, partitionF); // Bucket of Pf containing basis[k]
    bucket *bg = findCorrespondingBucket(*bf, partitionG); // Bucket of Pg corresponding to bf

    for (size_t ick = 0; ick < bg->bucketSize; ++ick) { // for c[k] in bg do... Where ck is the image element
        size_t ck = bg->elements[ick];
        if (generated_images[ck] == true) {
            continue;
        }
        _Bool problem = false;

        /* a very poor but effective way to handle the case when k = 0, because otherwise for k = 0 we get
         * (1L << (k-1)) == MAX_INT, and it loops figuratively forever */
        size_t LIMIT = k ? (1L << k) : 1;

        for (size_t linComb = 0; linComb < LIMIT; ++linComb) {
            size_t x = linComb ^basis[k];
            size_t y = ck;
            if (k) {
                for (size_t j = 0; j < k; ++j) {
                    if (1L << j & linComb) {
                        y ^= generated[basis[j]];
                    }
                }
            }
            bucket *bx = getBucket(x, fBucketPosition, partitionF); // Bucket that corresponds to x
            bucket *by = getBucket(y, gBucketPosition, partitionG); // Bucket that corresponds to y
            if (bx->bucketSize != by->bucketSize) {
                problem = true;
                break;
            }
            generated[x] = y;
            generated_images[y] = true;
        }
        if (!problem) {
            images[k] = ck;
            recursive(k + 1, images, partitionF, partitionG, n, generated, generated_images, permutation, fBucketPosition, gBucketPosition);
        }

        for (size_t linComb = 0; linComb < LIMIT; ++linComb) {
            size_t y = ck;
            if (k) {
                for (size_t j = 0; j < k; ++j) {
                    if ((1L << j) & linComb) {
                        y ^= generated[basis[j]];
                    }
                }
            }
            generated_images[y] = false;
        }
    }
    return NULL;
}
