#ifndef EA_EQUIVALENCE_DATACLASSES_H
#define EA_EQUIVALENCE_DATACLASSES_H

#include <stddef.h>

/**********
 * Hash Map
 *********/
typedef struct {
    size_t *key;
    size_t *value;
    size_t length;
} HashMap;

/**
 * Initialize a new Hash Map
 * @param size The size of the Hash Map
 * @return A new empty Hash Map
 */
HashMap *initHashMap(size_t size);

/**
 * Check if a hash map is empty
 * @param hashMap The hash map to check
 * @return True if empty, false otherwise
 */
bool isEmptyHashMap(HashMap *hashMap);

/**
 * Insert a new element to the hash map
 * @param hashMap The hash map to insert into
 * @param key The key of the element
 * @param value The value of the element
 */
void insertHashMap(HashMap *hashMap, size_t key, size_t value);

/**
 * Destroy a hash map
 * @param hashMap
 */
void destroyHashMap(HashMap *hashMap);


/*******
 * Set
 ******/
typedef struct {
    size_t *elements;
    size_t length;
} Set;

/**
 * Initialize a new set
 * @param size The size of the set
 * @return a new set
 */
Set *initSet(size_t size);

/**
 * Check if a set is empty
 * @param set The set to check
 * @return True if empty, false otherwise
 */
bool isEmptySet(Set *set);

/**
 * Insert element in a set
 * @param set The set to insertSet into
 * @param element The element to insertSet
 */
void insertSet(Set *set, size_t element);

/**
 * Print the elements in the set to the console
 * @param set The set to print
 */
void printSet(Set *set);

/**
 * @param set The set to destroy
 */
void destroySet(Set *set);

#endif //EA_EQUIVALENCE_DATACLASSES_H
