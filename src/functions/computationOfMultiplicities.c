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
void computationOfMultiplicities(truthTable * tt, int k) {
    if (k % 2 != 0) {
        printf("k is not even");
        exit(1);
    }

    size_t *multiplicities;
    multiplicities = malloc(sizeof(size_t) * tt->elements);

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

//    for (int i = 0; i < tt->elements; i++) {
//        printf("%zu\n", multiplicities[i]);
//    }

    size_t *result;
    result = calloc(tt->elements, sizeof(size_t));
    int current = multiplicities[0];
    int count = 0;
    int uniques = 0;
    for (int i = 0; i < tt->elements; ++i) {
        if (multiplicities[i] != current) {
            result[uniques] = count;
            current = multiplicities[i];
            count = 0;
            uniques += 1;
        }
        count += 1;
    }
    result[uniques] = count;
    qsort(&result[0], uniques + 1, sizeof(size_t), compare);
    printf("Multiplicities: ");
    for (int i = 0; i < uniques + 1; ++i) {
        printf("%zu ", result[i]);
    }
}
