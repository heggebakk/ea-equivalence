#include <malloc.h>
#include <stdbool.h>
#include "innerPermutation.h"

void innerPermutation(truthTable f, truthTable g) {
    bool *map = computeDomain(f, g, 1);
    computeSetOfTs(map, f);
}

bool * computeDomain(truthTable f, truthTable g, size_t x) {
    size_t n = f.dimension;
    bool *map = malloc(sizeof(bool) * 1L << n);
    for (int i = 0; i < 1L << n; ++i) {
        map[i] = false;
    }

    for (size_t y = 0; y < 1L << n; ++y) {
        size_t z = x ^ y;
        size_t sum = x ^ y ^ z;
        if (sum == 0) {
            size_t t = g.elements[x] ^ g.elements[y] ^ g.elements[z];
            map[t] = true;
        }
    }

    printf("Map of all t's: ");
    for (int i = 0; i < 1L << n; ++i) {
        printf("%d ", map[i]);
    }
    printf("\n");

    return map;
}

void computeSetOfTs(const bool *map, truthTable f) {
    size_t n = f.dimension;
    bool *initialSet = malloc(sizeof(bool) * 1L << n);
    for (int i = 0; i < 1L << n; ++i) {
        initialSet[i] = true;
    }
    for (int t = 0; t < 1L << n; ++t) {
        if (map[t]) {
            bool *tempSet = malloc(sizeof(bool) * 1L << n);
            for (size_t x = 0; x < 1L << n; ++x) {
                for (int i = 0; i < 1L << n; ++i) {
                    tempSet[i] = false;
                }
                for (size_t y = 0; y < 1L << n; ++y) {
                    size_t z = x ^ y;
                    size_t sum = x ^ y ^ z;
                    if (sum == 0 && (t == f.elements[x]) ^ f.elements[y] ^ f.elements[z]) {
                        tempSet[x] = true;
                        tempSet[y] = true;
                        tempSet[z] = true;
                    }
                }
            }
            for (int i = 0; i < 1L << n; ++i) {
                if (initialSet[i] != tempSet[i]) initialSet[i] = false;
            }
            printf("Tempset: ");
            for (int i = 0; i < 1L << n; ++i) {
                printf("%d ", tempSet[i]);
            }
            printf("\n");
            printf("initset: ");
            for (int i = 0; i < 1L << n; ++i) {
                printf("%d ", initialSet[i]);
            }
            printf("\n");
        }
    }
}
