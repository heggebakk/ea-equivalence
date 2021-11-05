#include <stdlib.h>
#include "structs.h"

truthTable inverse(truthTable function) {
    truthTable result;
    result.dimension = function.dimension;
    result.elements = calloc(sizeof(size_t), 1L << function.dimension);
    for (size_t x = 0; x < 1L << function.dimension; ++x) {
        size_t y = function.elements[x];
        result.elements[y] = x;
    }
    return result;
}
