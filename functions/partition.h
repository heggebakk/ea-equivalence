#ifndef EA_EQUIVALENCE_PARTITION_H
#define EA_EQUIVALENCE_PARTITION_H

#include <stdbool.h>
#include "../utils/truthTable.h"
#include "../utils/linkedList.h"

typedef struct vbfPartitions {
    size_t numberOfClasses;
    size_t *multiplicities;
    size_t *classSizes;
    size_t **classes;
} partitions;

/**
 * Partition bucket size where k = even
 * Works only for k = 4
 * @param function The function to be partitioned
 * @param k The size of the tuple T
 * @return Partitions
 */
partitions *partitionFunction(truthTable *function, size_t k);

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

/**
 * To avoid recalculating which permutation class maps to which, we'll create a map. This map will tell us which class
 * in partition F maps to a class in partition G. We can end up in a scenario where we have two classes in a permutation
 * with the same size. Then we have to try different mappings. If we get a result, then the mapping is correct, otherwise
 * we need to try a different mapping.
 * @param partitionF A partition of function F
 * @param partitionG A partition of function G
 */
size_t ** mapPartitionClasses(partitions *partitionF, partitions *partitionG);

void createMappings(size_t **mappings, struct Node **domains, partitions *partitionG, size_t numOfMappings);

void selectRecursive(size_t i, size_t *newList, bool *chosen, struct Node **domains, partitions *partitionG);

size_t factorial(size_t value);

#endif //EA_EQUIVALENCE_PARTITION_H
