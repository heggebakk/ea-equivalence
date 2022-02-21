#include <malloc.h>
#include <stdbool.h>
#include "dataStructures.h"

/***********
 * Partition
 **********/

/**********
 * HASH MAP
 *********/
HashMap *initHashMap(size_t size) {
    HashMap *newHashMap = malloc(sizeof(HashMap));
    newHashMap->length = 0;
    newHashMap->value = malloc(sizeof(size_t) * size);
    newHashMap->key = malloc(sizeof(size_t) * size);
    return newHashMap;
}

bool isEmptyHashMap(HashMap *hashMap) {
    return hashMap->length == 0;
}

void insertHashMap(HashMap *hashMap, size_t key, size_t value) {
    hashMap->key[hashMap->length] = key;
    hashMap->value[hashMap->length] = value;
    hashMap->length += 1;
}

void printHashMap(HashMap *hashMap) {
    printf("[");
    for (int i = 0; i < hashMap->length; ++i) {
        if ( i == hashMap->length - 1) {
            printf("(%zu, %zu)", hashMap->key[i], hashMap->value[i]);
        } else {
            printf("(%zu, %zu), ", hashMap->key[i], hashMap->value[i]);
        }
    }
    printf("]\n");
}

void destroyHashMap(HashMap *hashMap) {
    free(hashMap->key);
    free(hashMap->value);
    free(hashMap);
}

/*********
 * SET
 ********/
Set *initSet(size_t size) {
    Set *newSet = malloc(sizeof(Set));
    newSet->length = 0;
    newSet->elements = malloc(sizeof(size_t) * size);
    return newSet;
}

bool isEmptySet(Set *set) {
    return set->length == 0;
}

void insertSet(Set *set, size_t element) {
    for (int i = 0; i < set->length; ++i) {
        // If the element is in the set, we won't insert
        if (set->elements[i] == element) {
            return;
        }
    }
    set->elements[set->length] = element;
    set->length += 1;
}

void printSet(Set *set) {
    for (int i = 0; i < set->length; ++i) {
        if (i == set->length - 1) {
            printf("%zu", set->elements[i]);
        } else {
            printf("%zu, ", set->elements[i]);
        }
    }
}

void destroySet(Set *set) {
    free(set->elements);
    free(set);
}