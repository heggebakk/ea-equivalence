#include <stdlib.h>
#include <stdbool.h>
#include "permutation.h"
#include "time.h"

size_t *basis;

void outerPermutation(partitions f, partitions g) {
    double timeSpentPermutation = 0.0;
    clock_t startPermutation = clock();

    size_t n = f.dimension;
    basis = createBasis(n);

    size_t *images = malloc(sizeof(size_t) * (1L << n));
    size_t *generated = malloc(sizeof(size_t) *  (1L << n));
    // Assign each position in generated with 0
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
    recursive(0, images, f, g, n, generated, generated_images, permutation);
    printf("\nNum of permutations: %zu\n", permutation->numPermutations);

    bool bijective = isBijective(permutation, n);
    printf("The permutations is bijective: %s\n", (bijective ? "true" : "false"));
//    bool linear = isLinear(permutation, n);

    clock_t endPermutation = clock();
    timeSpentPermutation += (double) (endPermutation - startPermutation) / CLOCKS_PER_SEC;
    printf("Time spent Permutation: %f seconds\n", timeSpentPermutation);

    freePermutations(permutation);
    free(images);
    free(basis);
    free(generated);
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
    if (k == n) {

    }
    return true;
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

bucket *findBucket(size_t b, partitions function) {
    for (size_t i = 0; i < function.numBuckets; ++i) {
        bucket *bucket = &*function.buckets[i];
        for (size_t j = 0; j < bucket->bucketSize; ++j) {
            if (bucket->elements[j] == b) {
                return bucket;
            }
        }
    }
    printf("Bucket not found");
    return NULL;
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
           bool *generated_images, permutations *permutation) {
    if (k == n) {
        permutation[permutation->numPermutations].permutations = malloc(sizeof(size_t) * 1L << n);
        for (size_t i = 0; i < (1L << n); ++i) {
            permutation[permutation->numPermutations].permutations[i] = generated[i];
        }
        permutation->numPermutations += 1;
        return permutation;
    }
    bucket *bf = findBucket(basis[k], partitionF); // Bucket of Pf containing basis[k]
    bucket *bg = findCorrespondingBucket(*bf, partitionG); // Bucket of Pg corresponding to bf
    for (size_t ick = 0; ick < bg->bucketSize; ++ick) { // for c[k] in bg do.. Where ck is the image element
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
            bucket *bx = findBucket(x, partitionF); // Bucket that corresponds to x
            bucket *by = findBucket(y, partitionG); // Bucket that corresponds to y
            if (bx->bucketSize != by->bucketSize) {
                problem = true;
                break;
            }
            generated[x] = y;
            generated_images[y] = true;
        }
        if (!problem) {
            images[k] = ck;
            recursive(k + 1, images, partitionF, partitionG, n, generated, generated_images, permutation);
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
}
