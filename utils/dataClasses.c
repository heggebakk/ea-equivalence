#include <malloc.h>
#include <stdbool.h>
#include "dataClasses.h"

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