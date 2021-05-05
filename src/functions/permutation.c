#include <stdlib.h>
#include "permutation.h"
#include "math.h"

int *basis;


void outerPermutation(partitions f, partitions g) {
    basis = createBasis(f.dimension);
    int n = (int) f.dimension;

    size_t k = 1L << n;
    printf("k = %zu", k);
    size_t sizeBasis = 1L << k;
    printf("Sizebasis is = %zu", sizeBasis);
    imagesOfElements *images;
    images = malloc(sizeof(imagesOfElements));
    images->elements = (size_t *) malloc(sizeof(size_t) * sizeBasis);
    images->size = 0;
    int *generated = malloc(sizeof(int) * (int) pow(2, (double) f.dimension));
    // Assign each position in generated with 0
    for (int i = 0; i < pow(2, (double) f.dimension); ++i) {
        generated[i] = 0;
    }
    size_t linearCombinationsL = 0;
    size_t linearCombinationsM = 0;

    recursive(0, images, f, g, n, generated, linearCombinationsL, linearCombinationsM);

    freeImagesOfElements(images);
    free(basis);
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

void
recursive(int k, imagesOfElements *images, partitions partitionF, partitions partitionG, int n, int *generated,
          size_t linearCombinationsL, size_t linearCombinationM) {
    if (k == n) {
        return;
    }
    bucket *bf = findBucket(basis[k], partitionF); // Bucket of Pf containing basis[k]
    bucket *bg = findCorrespondingBucket(*bf, partitionG); // Bucket of Pg corresponding to bf
    for (size_t ck = 0; ck < bg->bucketSize; ++ck) { // for c[k] in bg do.. Where ck is the image element
        if (generated[ck] == 1) {
            continue;
        }
        for (size_t linComb = 0; linComb < (int) pow(2, n); ++linComb) {
            size_t x = linearCombinationsL + basis[k];
            size_t y = linearCombinationM + ck;
            bucket *bx = findBucket((int) x, partitionF); // Bucket that corresponds to x
            bucket *by = findBucket((int) y, partitionG); // Bucket that corresponds to y
            if (bx->bucketSize != by->bucketSize) {
                continue;
            }

        }
        if (generated[ck] == 0) {
            images->elements[images->size] = ck;
            images->size += 1;
        }
    }
    recursive(k + 1, images, partitionF, partitionG, n, generated, linearCombinationsL, 0);
}
