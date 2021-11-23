#ifndef EA_EQUIVALENCE_PARTITION_H
#define EA_EQUIVALENCE_PARTITION_H
#include "../utils/structs.h"

/**
 * Partition bucket size where k = even
 * Works only for k = 4
 * @param function The function to be partitioned
 * @param k The size of the tuple T
 * @param target An element of F2^n
 * @return Partitions
 */
partitions * partitionFunction(truthTable *function, size_t k, size_t target);

/**
 * A recursive function that find the multiplicities from k by xor'ing the elements from a function
 * @param k Number of recursions
 * @param i Number in recursion
 * @param multiplicities A list of multiplicities
 * @param function The function to check
 * @param x x_0 xor x_1 xor ... xor x_k
 * @param value The element[x] from the function where x is x_0 xor x_1 xor ... x_k
 */
void findAllMultiplicities(size_t k, int i, size_t *multiplicities, truthTable *function, size_t x, size_t value);

/**
 * Printing out to the console, information about a partition (The multiplicity of a "bucket, the size of the
 * bucket and the elements in the bucket.
 * @param p A partition
 */
void printPartitionInfo(partitions *p);

#endif //EA_EQUIVALENCE_PARTITION_H
