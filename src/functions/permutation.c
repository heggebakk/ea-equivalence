#include <stdlib.h>
#include "permutation.h"
#include "math.h"
typedef struct vbfPartitions partitions;
typedef struct vbfBucket bucket;

/**
 * Create a basis from the Identity matrix representation, but represented as integers.
 * @param dim The dimension
 * @return A list of binary representation in integers equal to the Identity matrix.
 */
int * createBasis(int dim);
/**
 * Find the bucket which holds the element b
 * @param b The element
 * @param partitions The partition holds all de buckets
 * @return The bucket where the element b lays
 */
bucket findBucket(int b, partitions partitions);

/**
 * Find at least one bucket which holds equal amount of elements
 * @param bucket The bucket to compare with
 * @param function The partition contains all the buckets the search over
 * @return At least one bucket
 */
bucket findCorrespondingBucket(bucket bucket, partitions function);

void outerPermutation(partitions f, partitions g) {
    int *basis = createBasis(f.dimension);
    int sizeBasis = (int) pow(2, f.dimension);

    for (int b1 = 0; b1 < sizeBasis; ++b1) {
        bucket bucketB1 = findBucket(basis[b1], f);
        bucket bucketC1 = findCorrespondingBucket(bucketB1, g);
        for (int c1 = 0; c1 < bucketC1.size; ++c1) {
            for (int b2 = 0; b2 < sizeBasis; ++b2) {
                bucket bucketB2 = findBucket(basis[b2], f);
                bucket bucketC2 = findCorrespondingBucket(bucketB2, g);
                for (int c2 = 0; c2 < bucketC2.size; ++c2) {
                    bucket bucketB3 = findBucket(basis[b1 + b2], f);
                    bucket bucketC3 = findBucket(basis[c1 + c2], g);
                    if (bucketB3.size != bucketC3.size) {
                        continue;
                    }
                }
            }
        }
//        for (int b2 = 0; b2 < sizeBasis; ++b2) {
//            bucket B3 = findBucket(basis[b1 + b2], f);
//            bucket C3 = findBucket(basis[b1 + b2], g);
//            if (B3.size == C3.size) {
//                // TODO: Do something
//                break;
//            }
//        }
    }
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

bucket findBucket(int b, partitions partitions) {
    for (int i = 0; i < partitions.numBuckets; ++i) {
        bucket bucket = partitions.buckets[i];
        for (int j = 0; j < bucket.size; ++j) {
            if (bucket.elements[j] == b) {
                return bucket;
            }
        }
    }
    bucket result;
    return result;
}

int * createBasis(int dimension) {
    int *basis;
    basis = malloc(sizeof(int) * dimension);
    for (int i = 0; i < dimension; ++i) {
        basis[i] = (int) pow(2, i);
    }
    return basis;
}
