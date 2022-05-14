#ifndef EA_EQUIVALENCE_STRUCTURES_H
#define EA_EQUIVALENCE_STRUCTURES_H

#include <stdbool.h>
#include "stdio.h"

/**
 * In this file you will find all the structures that is needed and used for all the different structures.
 */

/**
 * A Truth Table of a function F.
 * Holds the dimension, n, and all the elements of the function (2^n elements)
 */
typedef struct TruthTable {
    size_t dimension;
    size_t *elements;
} TruthTable;


/**
 * Holds all the information for the Partition:
 * Number of buckets, all the multiplicities, the sizes of the buckets, and all the elements in all the buckets.
 */
typedef struct {
    size_t numberOfClasses;
    size_t *multiplicities;
    size_t *classSizes;
    size_t **classes;
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
 * Write a Truth Table to a file
 * @param tt The truth table
 * @param fp The file to write to
 * @param ttName The name of the truth table represented as a String
 */
void writeTruthTable(FILE *fp, TruthTable *tt);

/**
 * Parse a file to a Truth Table Struct
 * The first line of the file must be the numberOfClasses, the next line should be all the elements seperated with white space
 *
 * Example:
 * 6
 * 0 1 8 15 27 14 35 48 53 39 43 63 47 41 1 1 41 15 15 47 52 6 34 22 20 33 36 23 8 41 8 47 36 52 35 53 35 39 20 22 33 34 48 53 39 48 6 23 22 33 63 14 23 52 14 43 27 63 36 6 27 43 20 34
 *
 * @param filename The filename to parse
 * @return A Truth Table struct with the parsed data
 */
TruthTable *parseTruthTable(const char filename[64]);

Partition *initPartition(size_t size);

/**
 * Partition bucket size where k = even
 * Works only for k = 4
 * @param function The function to be partitioned
 * @param k The size of the tuple T
 * @return Partitions
 */
Partition *partitionFunction(TruthTable *function, size_t k);

/**
 * Printing out to the console, information about a partition (The multiplicity of a "bucket, the size of the
 * bucket and the elements in the bucket.
 * @param partition A partition
 */
void printPartitionInfo(Partition *partition);

/**
 * Write partition information to a file where
 * First line, n, is the number of classes
 * Next n lines contains the class n
 * @param partition The partition to write
 * @param fp The file to write to
 */
void writePartition(Partition *partition, FILE *fp);

/**
 * To avoid recalculating which permutation class maps to which, we'll create a map. This map will tell us which class
 * in partition F maps to a class in partition G. We can end up in a scenario where we have two classes in a permutation
 * with the same size. Then we have to try different mappings. If we get a result, then the mapping is correct, otherwise
 * we need to try a different mapping.
 * @param partitionF A partition of function F
 * @param partitionG A partition of function G
 */
void **mapPartitionClasses(Partition *partitionF, Partition *partitionG, size_t dimension, MappingOfBuckets *mappingOfClasses);

void destroyPartitions(Partition *p);

MappingOfBuckets *initMappingsOfClasses();

void addToMOC(MappingOfBuckets *moc, size_t *mappings, size_t *domains, size_t mappingSize, size_t domainSize);

void destroyMappingOfClasses(MappingOfBuckets *mappingsOfClasses);

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
createMappings(MappingOfBuckets *mappingOfClasses, Node **domains, Partition *partitionG, size_t dimension, size_t numOfMappings);

/**
 * Initialize a new linked list of struct Node
 * @return A new empty linked list
 */
Node * initLinkedList();

/**
 * Add a new node to a linked list.
 * @param head The head of the linked list
 * @param data The data for the node to be added
 */
void addToLinkedList(Node *head, size_t data);

/**
 * Print a linked list to the console
 * @param head The head of the linked list
 */
void printLinkedList(Node *head);

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
void destroyLinkedList(Node *head);

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
void addNode(TtNode *head, TruthTable *data);

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
TruthTable * getNode(TtNode *head, size_t index);

/**
 * Print linked list to the console
 * @param head The head of the linked list
 */
void printTtLinkedList(TtNode *head);

/**
 * Write all truth tables from a linked list to a file
 * @param head The head of the linked list
 * @param fp The file to write to
 */
void writeTtLinkedList(TtNode *head, FILE *fp);

/**
 * Destroy Linked List of struct TtNode
 * @param head The head of the linked list
 */
void destroyTtLinkedList(TtNode *head);

void
selectRecursive(size_t i, size_t *newList, size_t *currentDomain, bool *chosen, Node **domains, Partition *partitionG,
                size_t dimension, MappingOfBuckets *moc);

/**
 * Factorial calculations
 * @param value The value to do factorial on
 * @return The factorial of the value
 */
size_t factorial(size_t value);

#endif //EA_EQUIVALENCE_STRUCTURES_H
