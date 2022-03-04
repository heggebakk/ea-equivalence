#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "walshTransform.h"

void destroyWalshTransform(walshTransform *wt) {
    for (size_t i = 0; i < 1L << wt->dimension; ++i) {
        free(wt->elements[i]);
    }
    free(wt->elements);
    free(wt);
}

_Bool dot(size_t a, size_t b) {
    return __builtin_popcountl(a & b) % 2;
}

size_t truthTableWalshTransform(truthTable tt, size_t a, size_t b) {
    size_t sum = 0;
    for (size_t x = 0; x < 1L << tt.dimension; ++x) {
        sum += dot(a, x) ^ dot(b, tt.elements[x]) ? -1 : 1;
    }
    return sum;
}

walshTransform * truthTableToWalshTransform(truthTable tt) {
    walshTransform *wt = malloc(sizeof(walshTransform));
    size_t dimension = tt.dimension;
    wt->dimension = dimension;
    wt->elements = malloc(sizeof(size_t) * 1L << dimension);
    for (size_t a = 0; a < 1L << dimension; ++a) {
        wt->elements[a] = malloc(sizeof(size_t) * 1L << dimension);
        for (size_t b = 0; b < 1L << dimension; ++b) {
            wt->elements[a][b] = (size_t) truthTableWalshTransform(tt, a, b);
        }
    }
    return wt;
}

void printWalshTransformTable(walshTransform wt) {
    printf("Walsh transform: \n");
    size_t entries = 1L << wt.dimension;
    for (size_t a = 0; a < entries; ++a) {
        for (size_t b = 0; b < entries; ++b) {
            printf("%ld\t", wt.elements[a][b]);
        }
        printf("\n");
    }
}

size_t walshTransformPowerMoment(walshTransform wt, size_t k, size_t shiftA, size_t shiftB) {
    size_t sum = 0;
    for (size_t a = 0; a < 1L << wt.dimension; ++a) {
        for (size_t b = 0; b < 1L << wt.dimension; ++b) {
            size_t modifier = (dot(a, shiftA) ^ dot(b, shiftB)) ? -1 : 1;
            size_t base = wt.elements[a][b];
            size_t product = 1;
            for (size_t i = 0; i < k; ++i) {
                product *= base;
            }
            sum += product * modifier;
        }
    }
    return sum;
}

partitions *eaPartitionWalsh(truthTable *tt, size_t k) {
    // Change function f and g from a truth table to a walsh transform table
    walshTransform *wt = truthTableToWalshTransform(*tt);

    size_t *multiplicities = malloc(sizeof(size_t) * 1L << wt->dimension);
    size_t possibleValues [1L << wt->dimension];
    size_t currentPossibleValue = 0;

    for (size_t s = 0; s < 1L << wt->dimension; ++s) {
        size_t walsh = walshTransformPowerMoment(*wt, k, 0, s);
        // Note that the value of the power moment is 2^(2*n) times the multiplicity
        size_t multiplicity = walsh >> (2 * wt->dimension);
        multiplicities[s] = multiplicity;
        // Try to find this multiplicity in the list of recorded multiplicities. If it's note there, add a new entry.
        size_t k2 = 0;
        for (; k2 < currentPossibleValue; ++k2) {
            if (possibleValues[k2] == multiplicity) {
                break;
            }
        }
        if (k2 == currentPossibleValue) {
            possibleValues[currentPossibleValue++] = multiplicity;
        }
    }

    // At this point, we know what exact multiplicities define the partition, and can construct the structure
    partitions *p = malloc(sizeof(partitions));
    p->numberOfClasses = currentPossibleValue;
    p->classSizes = malloc(sizeof(size_t) * currentPossibleValue);
    p->classes = malloc(sizeof(size_t *) * currentPossibleValue);
    p->multiplicities = multiplicities;

    size_t currentClass[1L << wt->dimension];
    size_t currentClassIndex;

    for (size_t classIndex = 0; classIndex < currentPossibleValue; ++classIndex) {
        currentClassIndex = 0;
        for (size_t s = 0; s < 1L << wt->dimension; ++s) {
            if (multiplicities[s] == possibleValues[classIndex]) {
                currentClass[currentClassIndex++] = s;
            }
        }
        p->classSizes[classIndex] = currentClassIndex;
        p->classes[classIndex] = malloc(sizeof(size_t *) * currentClassIndex);
        memcpy(p->classes[classIndex], currentClass, sizeof(size_t) * currentClassIndex);
    }
    destroyWalshTransform(wt);
    return p;
}
