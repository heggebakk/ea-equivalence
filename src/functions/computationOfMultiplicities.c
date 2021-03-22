#include <stdlib.h>
#include <stdio.h>
#include "computationOfMultiplicities.h"

typedef struct vbfTruthTable truthTable;

int compare(const void *a, const void *b) {
    return (*(int *) a - *(int *) b);
}

/*
 * Works only for k = 4
 * TODO: Make it work for all k = even
 */
void computationOfMultiplicities(truthTable * tt) {
    size_t *multiplicities;
    multiplicities = malloc(sizeof(int) * tt->elements);

    for (int i = 0; i < tt->elements; ++i) {
        multiplicities[i] = 0;
    }

    for (int x1 = 0; x1 < tt->elements; ++x1) {
        for (int x2 = 0; x2 < tt->elements; ++x2) {
            for (int x3 = 0; x3 < tt->elements; ++x3) {
                int x4 = x1 ^ x2 ^ x3;
                int value = tt->array[x1] ^ tt->array[x2] ^ tt->array[x3] ^ tt->array[x4];
                multiplicities[value] += 1;
            }
        }
    }

    qsort(&multiplicities[0], tt->elements, sizeof(size_t), compare);

    for (int i = 0; i < tt->elements; i++) {
        printf("%zu\n", multiplicities[i]);
    }
}
