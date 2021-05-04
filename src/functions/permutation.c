#include <stdlib.h>
#include "permutation.h"
#include "math.h"
int *basis;

void outerPermutation(partitions f, partitions g) {
    basis = createBasis(f.dimension);
    int n = (int) f.dimension;

    size_t sizeBasis = (size_t) pow(pow(2, n), n);
    printf("Size basis: %zu\n", sizeBasis);
    struct imagesOfElements *images;
    images = malloc(sizeof(struct imagesOfElements));
    images->elements = (size_t *) malloc(sizeof(size_t) * sizeBasis);
    images->size = 0;
    int *generated = malloc(sizeof(int) * sizeBasis);

    recursive(0, images, f, g, n, generated);
    free(images->elements);
    free(images);
}

bucket findCorrespondingBucket(bucket bucketB, partitions function) {
    for (int i = 0; i < function.numBuckets; ++i) {
        bucket currentBucket = function.buckets[i];
        if (bucketB.size == currentBucket.size) {
            return currentBucket;
        }
    }
    bucket result;
    return result;
}

bucket findBucket(int b, partitions function) {
    for (int i = 0; i < function.numBuckets; ++i) {
        bucket bucket = function.buckets[i];
        for (int j = 0; j < bucket.size; ++j) {
            if (bucket.elements[j] == b) {
                return bucket;
            }
        }
    }
    bucket result;
    return result;
}

int * createBasis(size_t dimension) {
    basis = malloc(sizeof(int) * dimension);
    for (int i = 0; i < dimension; ++i) {
        basis[i] = (int) pow(2, i);
    }
    return basis;
}

void recursive(int k, struct imagesOfElements *images, partitions partitionF, partitions partitionG, int n,
               int *generated) {
    if (k == n) {
//        for (int i = 0; i < images->size; ++i) {
//            printf("Images of elements: %zu ", images->elements[i]);
//        }
        printf("\nDone\n");
        return;
    }
    bucket bf = findBucket(basis[k], partitionF); // Bucket of Pf containing basis[k]
    bucket bg = findCorrespondingBucket(bf, partitionG); // Bucket of Pg corresponding to bf
    for (int c = 0; c < bg.size; ++c) { // for c[k] in bg do..
//        printf("size: %zu\n", images->size);
//        printf("wat: %zu", bg.elements[basis[c]]);
//        images->elements[images->size] = bg.elements[basis[c]];
        images->size += 1;
    }
    recursive(k + 1, images, partitionF, partitionG, n, generated);
}