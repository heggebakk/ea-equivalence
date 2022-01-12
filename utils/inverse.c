#include <stdlib.h>
#include "truthTable.h"

/**
 * Create the inverse function
 * @param function The function to create the inverse of
 * @return The inverse
 */
truthTable * inverse(truthTable function) {
    truthTable *result = malloc(sizeof(truthTable));
    result->dimension = function.dimension;
    result->elements = calloc(sizeof(size_t), 1L << function.dimension);
    for (size_t x = 0; x < 1L << function.dimension; ++x) {
        size_t y = function.elements[x];
        result->elements[y] = x;
    }
    return result;
}
