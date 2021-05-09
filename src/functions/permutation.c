#include <stdlib.h>
#include <stdbool.h>
#include "permutation.h"
#include "math.h"

int *basis;

void outerPermutation(partitions f, partitions g) {
    basis = createBasis(f.dimension);
    size_t n = f.dimension;

    size_t *images = malloc(sizeof(size_t) * (1L << n));
    size_t *generated = malloc(sizeof(size_t) *  (1L << n));
    // Assign each position in generated with 0
    for (int i = 0; i < 64; ++i) {
        generated[i] = 0;
    }
    _Bool generated_images [(1L << n)];
//    printf("Upper bound is %lu\n", (1L << n));
    for(size_t i = 0; i < (1L << n); ++i) {
        generated_images[i] = false;
    }
    printf("Hello\n");
    generated_images[0] = true;
    recursive(0, images, f, g, n, generated, generated_images);

    (images);
    free(basis);
    free(generated);
}

bucket * findCorrespondingBucket(bucket bucketB, partitions function) {
    for (int i = 0; i < function.numBuckets; ++i) {
        bucket *currentBucket = &(*function.buckets[i]);
        if (bucketB.bucketSize == currentBucket->bucketSize) {
            return currentBucket;
        }
    }
    printf("Couldn't find a corresponding bucket to bucket with bucketSize %d", bucketB.bucketSize);
    return NULL;
}

bucket * findBucket(int b, partitions function) {
    for (int i = 0; i < function.numBuckets; ++i) {
        bucket *bucket = &(*function.buckets[i]);
        for (int j = 0; j < bucket->bucketSize; ++j) {
            if (bucket->elements[j] == b) {
                return bucket;
            }
        }
    }
    printf("Bucket not found");
    return NULL;
}

int * createBasis(size_t dimension) {
    basis = malloc(sizeof(size_t) * dimension + 1);
    for (int i = 0; i < dimension + 1; ++i) {
        basis[i] = (int) pow(2, i);
    }
    return basis;
}

void recursive(size_t k, size_t *images, partitions partitionF, partitions partitionG, size_t n, size_t *generated, _Bool * generated_images) {
    if (k == n) {
        printf("\nGenerated: \n");
        for (int i = 0; i < (1L << n); ++i) {
            printf("%lu ", generated[i]);
        }
        return;
    }
    bucket *bf = findBucket(basis[k], partitionF); // Bucket of Pf containing basis[k]
    bucket *bg = findCorrespondingBucket(*bf, partitionG); // Bucket of Pg corresponding to bf
    for (size_t ick = 0; ick < bg->bucketSize; ++ick) { // for c[k] in bg do.. Where ck is the image element
        size_t ck = bg->elements[ick];
        if (generated_images[ck] == true) {
            continue;
        }
        _Bool problem = false;
        size_t LIMIT = k ? (1L << (k)) : 1; /* a very poor but effective way to handle the case when k = 0, because
 * otherwise for k = 0 we get (1L << (k-1)) == MAX_INT, and it loops figuratively forever */
        for (size_t linComb = 0; linComb < LIMIT; ++linComb) {
            size_t x = linComb ^basis[k];
            size_t y = ck;
            if(k) {
                for (size_t j = 0; j < k; ++j) {
                    if ((1L << j) & linComb) {
                        y ^= generated[basis[j]];
                    }
                }
            }
            bucket *bx = findBucket((int) x, partitionF); // Bucket that corresponds to x
            bucket *by = findBucket((int) y, partitionG); // Bucket that corresponds to y
            if (bx->bucketSize != by->bucketSize) {
                problem = true;
                break;
            }
            generated[x] = y;
            generated_images[y] = true;
        }
        if(!problem) {
            images[k] = ck;
            recursive(k + 1, images, partitionF, partitionG, n, generated, generated_images);
        }

        for (size_t linComb = 0; linComb < LIMIT; ++linComb) {
            size_t y = ck;
            if(k) {
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
