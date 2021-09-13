#ifndef SRC_PERMUTATION_H
#define SRC_PERMUTATION_H
#include "../utils/structs.h"
#include "stdbool.h"

/**
 * Create a basis from the Identity matrix representation, but represented as integers.
 * @param dim The dimension
 * @return A list of binary representation in integers equal to the Identity matrix.
 */
size_t *createBasis(size_t dim);

/**
 * Create a list representing in which bucket we can find i
 * @param function The function with the partitions
 * @return The list of bucket representation
 */
size_t * correspondingBucket(partitions function);

/**
 * Get the bucket where we can find the k
 * @param k The k we are looking for
 * @param bucketPositions A list representing in which bucket the element k exists.
 * @param function The partition function
 * @return The bucket where we can find k
 */
bucket *getBucket(size_t k, const size_t *bucketPositions, partitions function);

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
permutations
*recursive(size_t k, size_t *images, partitions partitionF, partitions partitionG, size_t n, size_t *generated,
           bool *generated_images, permutations *permutation, size_t *pInt, size_t *pInt1);

/**
 *
 * @param f Function F
 * @param g Function G
 */
permutations * outerPermutation(struct vbfPartitions f, struct vbfPartitions g);

/**
 * Check whether the permutations are linear or not
 * @param permutation List of all permutations
 * @param k Counter to keep track of progress of recursion
 * @param n Dimension
 * @return True if the permutations is linear, false otherwise
 */
bool isLinear(permutations *permutation, size_t k, size_t n);

/**
 * Check whether the permutations is bijective or not
 * @param permutation List of all permutations
 * @param n Dimension
 * @return True if bijective, false otherwise
 */
bool isBijective(permutations *permutation, size_t n);


#endif //SRC_PERMUTATION_H
