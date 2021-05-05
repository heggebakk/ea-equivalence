#include <stdlib.h>
#include "permutation.h"
#include "math.h"
int *basis;

void outerPermutation(partitions f, partitions g) {
    basis = createBasis(f.dimension);
    int n = (int) f.dimension;

    size_t sizeBasis = (size_t) pow(pow(2, n), n);
    struct imagesOfElements *images;
    images = malloc(sizeof(struct imagesOfElements));
    images->elements = (size_t *) malloc(sizeof(size_t) * sizeBasis);
    images->size = 0;
    int *generated = malloc(sizeof(int) * (int) pow(2, (double) f.dimension));
    // Assign each position in generated with 0
    for (int i = 0; i < pow(2, (double) f.dimension); ++i) {
        generated[i] = 0;
    }

    recursive(0, images, f, g, n, generated);
    free(images->elements);
    free(images);
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

void recursive(int k, struct imagesOfElements *images, partitions partitionF, partitions partitionG, int n,
               int *generated) {
    if (k == n) {
        printf("\nDone\n");
        return;
    }
    bucket *bf = findBucket(basis[k], partitionF); // Bucket of Pf containing basis[k]
    bucket *bg = findCorrespondingBucket(*bf, partitionG); // Bucket of Pg corresponding to bf
    printf("Bg bucketSize: %d ", bg->bucketSize);
    for (int ck = 0; ck < bg->bucketSize; ++ck) { // for c[k] in bg do..
        if (generated[ck] == 1) continue;
    }
    recursive(k + 1, images, partitionF, partitionG, n, generated);
}
