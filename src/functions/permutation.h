#ifndef SRC_PERMUTATION_H
#define SRC_PERMUTATION_H
#include "../utils/structs.h"

/**
 * Create a basis from the Identity matrix representation, but represented as integers.
 * @param dim The dimension
 * @return A list of binary representation in integers equal to the Identity matrix.
 */
int * createBasis(size_t dim);

/**
 * Find the bucket which holds the element basis
 * @param basis The element
 * @param function The partition holds all de buckets
 * @return The bucket where the element basis lays
 */
bucket * findBucket(int basis, partitions function);

/**
 * Find at least one bucket which holds equal amount of elements
 * @param bucket The bucket to compare with
 * @param function The partition contains all the buckets the search over
 * @return At least one bucket
 */
bucket * findCorrespondingBucket(bucket bucket, partitions function);

/**
 * Recursive function
 * @param k
 * @param images
 * @param partitionF
 * @param partitionG
 * @param n
 * @param generated
 */
void
recursive(size_t k, size_t *images, partitions partitionF, partitions partitionG, int n, size_t *generated, _Bool *generated_images);

/**
 *
 * @param f Function F
 * @param g Function G
 */
void outerPermutation(struct vbfPartitions f, struct vbfPartitions g);

#endif //SRC_PERMUTATION_H
