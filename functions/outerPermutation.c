#include <stdbool.h>
#include "stdlib.h"
#include "outerPermutation.h"

size_t *basis;

permutations *outerPermutation(partitions f, partitions g) {
    size_t n = f.dimension;
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
    size_t *fBucketPosition = correspondingBucket(f);
    size_t *gBucketPosition = correspondingBucket(g);
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

size_t *correspondingBucket(partitions function) {
    size_t *list = (size_t *) malloc(sizeof(size_t) * 1L << function.dimension);
    for (size_t i = 0; i < 1L << function.dimension; ++i) {
        for (size_t j = 0; j < function.numBuckets; ++j) {
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

    bucket *bf = getBucket(basis[k], fBucketPosition, partitionF);
    bucket *bg = findCorrespondingBucket(*bf, partitionG);
    for (size_t ick = 0; ick < bg->bucketSize; ++ick) {
        size_t ck = bg->elements[ick];
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
            bucket *bx = getBucket(x, fBucketPosition, partitionF); // Bucket that corresponds to x
            bucket *by = getBucket(y, fBucketPosition, partitionG); // Bucket that corresponds to y
            if (bx->bucketSize != by->bucketSize) {
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

bucket *getBucket(size_t k, const size_t *bucketPositions, partitions function) {
    return function.buckets[bucketPositions[k]];
}

bucket *findCorrespondingBucket(bucket b, partitions function) {
    for (size_t i = 0; i < function.numBuckets; ++i) {
        bucket *currentBucket = &(*function.buckets[i]);
        if (b.bucketSize == currentBucket->bucketSize) {
            return currentBucket;
        }
    }
    printf("Couldn't find a corresponding bucket to bucket with bucket size %zu", b.bucketSize);
    exit(1);
}

bool isBijective(permutations *p, size_t n) {
    if (p->numPermutations <= 0) {
        printf("There are no permutations to check \n");
        return false;
    }
    for (size_t pi = 0; pi < p->numPermutations - 1; ++pi) {
        for (int pj = pi + 1; pj < p->numPermutations; ++pj) {
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
