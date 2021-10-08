#include <malloc.h>
#include <stdbool.h>
#include "innerPermutation.h"

void innerPermutation(truthTable f, truthTable g) {
    bool *map = computeSetOfTs(f, g, 1);
    size_t *domain = computeDomain(map, f);
    free(map);
    free(domain);
}

// x is basis
bool * computeSetOfTs(truthTable f, truthTable g, size_t x) {
    size_t n = f.dimension;
    bool *map = calloc(sizeof(bool), 1L << n);

    for (size_t y = 0; y < 1L << n; ++y) {
        size_t t = g.elements[x] ^ g.elements[y] ^ g.elements[x ^ y];
        map[t] = true;
    }
    return map;
}

size_t * computeDomain(const bool *listOfTs, truthTable f) {
    size_t n = f.dimension;
    bool *domain = malloc(sizeof(bool) * 1L << n);
    for (size_t i = 0; i < 1L << n; ++i) {
        domain[i] = true;
    }
    for (int t = 0; t < 1L << n; ++t) {
        if (listOfTs[t]) {
            bool *tempSet = calloc(sizeof(bool), 1L << n);
            for (size_t x = 0; x < 1L << n; ++x) {
                for (size_t y = 0; y < 1L << n; ++y) {
                    size_t z = x ^ y;
                    if (t == (f.elements[x] ^ f.elements[y] ^ f.elements[x ^ y])) {
                        tempSet[x] = true;
                        tempSet[y] = true;
                        tempSet[x ^ y] = true;
                    }
                }
            }
            for (size_t i = 0; i < 1L << n; ++i) {
                domain[i] &= tempSet[i];
            }
            free(tempSet);
        }
    }

    // Represent the restricted domain in an array
    // TODO: Change to a linked list
    size_t *results = calloc(sizeof(size_t), 1L << n);
    size_t numResult = 0;
    for (size_t i = 0; i < 1L << n; ++i) {
        if (domain[i]) {
            results[numResult] = i;
            numResult += 1;
        }
    }
    free(domain);
    return results;
}
