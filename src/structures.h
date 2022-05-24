#ifndef EA_EQUIVALENCE_STRUCTURES_H
#define EA_EQUIVALENCE_STRUCTURES_H

#include <stdbool.h>
#include "stdio.h"
#include "time.h"

/**
 * In this file you will find all the structures that is needed and used for all the different structures.
 */

/**
 * A Truth Table of a function F.
 * Holds the n, n, and all the elements of the function (2^n elements)
 */
typedef struct TruthTable {
    size_t n; // Dimension of the function
    size_t *elements; // All the elements of the function
} TruthTable;


/**
 * Holds all the information for the Partition:
 * Number of buckets, all the multiplicities, the sizes of the buckets, and all the elements in all the buckets.
 */
typedef struct Partition {
    size_t numberOfBuckets; // Total number of partition buckets
    size_t *multiplicities; // A list of all the multiplicities
    size_t *bucketSizes; // The size of each bucket, given as a list
    size_t **buckets; // A list of all the elements in each bucket.
} Partition;

typedef struct MappingOfBuckets {
    size_t **mappings;
    size_t **domains;
    size_t numOfMappings;
} MappingOfBuckets;

/**
 * Represent a node of a linked list
 */
typedef struct Node {
    size_t data;
    struct Node *next;
} Node ;

typedef struct TtNode {
    TruthTable *data;
    struct TtNode *next;
} TtNode ;

typedef struct WalshTransform {
    size_t dimension;
    size_t **elements;
} WalshTransform;

/**
 * A struct for keeping all the run times for the entire program
 */
typedef struct RunTimes {
    double total;
    double partition;
    double outerPermutation;
    double innerPermutation;
} RunTimes;

/**
 * Initialize a new Truth Table
 * @param n The n
 * @return A new empty truth table
 */
TruthTable *initTruthTable(size_t n);

/**
 * Destroy a Truth table
 * @param tt The truth table to be destroyed
 */
void destroyTruthTable(TruthTable *tt);

/**
 * Print out the information stored in the Truth Table struct to the console
 * @param tt The Truth Table to print out
 */
void printTruthTable(TruthTable *tt);

/**
 * Parse a file to a Truth Table Struct
 * The first line of the file must be the numberOfBuckets, the next line should be all the elements seperated with white space
 *
 * Example:
 * 6
 * 0 1 8 15 27 14 35 48 53 39 43 63 47 41 1 1 41 15 15 47 52 6 34 22 20 33 36 23 8 41 8 47 36 52 35 53 35 39 20 22 33 34 48 53 39 48 6 23 22 33 63 14 23 52 14 43 27 63 36 6 27 43 20 34
 *
 * @param filename The filename to parse
 * @return A Truth Table struct with the parsed data
 */
TruthTable *parseTruthTable(const char filename[64]);

/**
 * Initialize a new Partition. This method takes in the n (n) of the function and created a new Partition where
 * it allocates all the memory needed to create all the lists.
 * @param n The n of the field we are working in
 * @return A pointer to a new Partition
 */
Partition *initPartition(size_t n);

/**
 * Partition a function F. This function will find the bucket partition for each element in F.
 * @param F The truth table of a function.
 * @param k The size of the tuple T
 * @return A new Partition of function F
 */
Partition *partitionFunction(TruthTable *F, size_t k);

/**
 * Printing out to the console. The information about a partition (The multiplicity of a "bucket, the size of the
 * bucket and the elements in the bucket.
 * @param partition A partition of a function
 */
void printPartition(Partition *partition);

/**
 * Free the memory allocated for the Partition
 * @param partition The Partition to destroy
 */
void destroyPartitions(Partition *partition);

/**
 * To avoid recalculating which permutation bucket maps to which, we'll create a map. This map will tell us which bucket
 * in partition F maps to a bucket in partition G. We can end up in a scenario where we have two buckets in a permutation
 * with the same size. Then we have to try different mappings. If we get a result, then the mapping is correct, otherwise
 * we need to try a different mapping.
 * @param partitionF A partition of function F
 * @param partitionG A partition of function G
 */
void **mapPartitionBuckets(Partition *partitionF, Partition *partitionG, size_t dimension, MappingOfBuckets *mappingOfBuckets);

MappingOfBuckets *initMappingsOfBuckets();

void addToMOC(MappingOfBuckets *moc, size_t *mappings, size_t *domains, size_t mappingSize, size_t domainSize);

void destroyMappingOfBuckets(MappingOfBuckets *mappingsOfBuckets);

/**
 * A recursive function that find the multiplicities from k by xor'ing the elements from a function
 * @param k Number of recursions
 * @param i Number in recursion
 * @param multiplicities A list of multiplicities
 * @param function The function to check
 * @param x x_0 xor x_1 xor ... xor x_k
 * @param value The element[x] from the function where x is x_0 xor x_1 xor ... x_k
 */
void findAllMultiplicities(size_t k, int i, size_t *multiplicities, TruthTable *function, size_t x, size_t value);

void
createMappings(MappingOfBuckets *mappingOfBuckets, Node **domains, Partition *partitionG, size_t dimension, size_t numOfMappings);

/**
 * Initialize a new linked list of struct Node
 * @return A new empty linked list
 */
Node * initNode();

/**
 * Add a new node to a linked list.
 * @param head The head of the linked list
 * @param data The data for the node to be added
 */
void addToNode(Node *head, size_t data);

/**
 * Print a linked list to the console
 * @param head The head of the linked list
 */
void printNodes(Node *head);

/**
 * Counts all the nodes in a linked list and returns the result
 * @param head The head of the linked list
 * @return The number of nodes in the linked list
 */
size_t countNodes(Node *head);

/**
 * Destroy a linked list of struct Node
 * @param head The head of the Linked List
 */
void destroyNodes(Node *head);

/**
 * Initialize a new Linked list of type Truth Table Node
 * @return The new node created
 */
TtNode * initTtNode();

/**
 * Add a node to the linked list.
 * @param head The pointer to the head of the linked list
 * @param data The data to add to the list
 */
void addTtNode(TtNode *head, TruthTable *data);

/**
 * Counts all the nodes in a linked list and returns the result
 * @param head The head of the linked list
 * @return The number of nodes in the linked list
 */
size_t countTtNodes(TtNode *head);

/**
 * Count the number of nodes in the linked list
 * @param head The pointer to the head of the linked list
 * @return The number of nodes in the linked list
 */
TruthTable * getTtNode(TtNode *head, size_t index);

/**
 * Print linked list to the console
 * @param head The head of the linked list
 */
void printTtNodes(TtNode *head);

/**
 * Destroy Linked List of struct TtNode
 * @param head The head of the linked list
 */
void destroyTtNodes(TtNode *head);

void
selectRecursive(size_t i, size_t *newList, size_t *currentDomain, bool *chosen, Node **domains, Partition *partitionG,
                size_t dimension, MappingOfBuckets *moc);

/**
 * Factorial calculations
 * @param value The value to do factorial on
 * @return The factorial of the value
 */
size_t factorial(size_t value);

void destroyWalshTransform(WalshTransform *wt);

_Bool dot(size_t a, size_t b);

size_t truthTableWalshTransform(TruthTable tt, size_t a, size_t b);

WalshTransform *truthTableToWalshTransform(TruthTable tt);

void printWalshTransformTable(WalshTransform wt);

size_t walshTransformPowerMoment(WalshTransform wt, size_t k, size_t shiftA, size_t shiftB);

Partition *eaPartitionWalsh(TruthTable *tt, size_t k);

/**
 * Initialize a new empty struct of Run times
 * @return A empty RunTimes
 */
RunTimes *initRunTimes();

/**
 * Stop the time and return the run time
 * @param runTime The time to do the calculations on
 * @param startParsing The start time
 * @return The total run time
 */
double stopTime(double runTime, clock_t startParsing);

/**
 * Print all times to the console
 * @param runTimes the Run times to print
 */
void printTimes(RunTimes *runTimes);

/**
 * Destroy the run times
 * @param runTimes The run times to destroy
 */
void destroyRunTimes(RunTimes *runTimes);

/**
 * Create a standard basis for the given n. 2^n where n = n.
 * @param dimension The n to create the basis from
 * @return The standard basis of 2^n, where n = n
 */
size_t *createStandardBasis(size_t dimension);
#endif //EA_EQUIVALENCE_STRUCTURES_H
