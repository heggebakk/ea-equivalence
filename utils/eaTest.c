#include <stdlib.h>
#include <string.h>
#include "eaTest.h"
#include "../functions/innerPermutation.h"
#include "truthTable.h"

truthTable *randomLinearFunction(size_t dimension) {
    size_t entries = 1L << dimension;
    size_t listGenerated[entries];
    listGenerated[0] = 0;
    size_t basisImages[dimension];
    for (size_t i = 0; i < dimension; ++i) {
        size_t j = rand() % entries;
        basisImages[i] = j;
        for (int k = 0; k < 1L << i; ++k) {
            listGenerated[(1L << i) + k] = listGenerated[k] ^ j;
        }
    }
    truthTable *result = malloc(sizeof(truthTable));
    result->dimension = dimension;
    result->elements = malloc(sizeof(size_t) * entries);
    memcpy(result->elements, listGenerated, sizeof(size_t) * entries);
    return result;
}

truthTable *randomLinearPermutation(size_t dimension) {
    size_t entries = 1L << dimension;
    bool generated[entries];
    size_t listGenerated[entries];
    generated[0] = true;
    for (size_t i = 1; i < entries; ++i) {
        generated[i] = false;
    }
    listGenerated[0] = 0;

    size_t basisImages[dimension];
    for (int i = 0; i < dimension; ++i) {
        size_t j = rand() % entries;
        while (generated[j]) {
            j = (j + 1) % entries;
        }
        basisImages[i] = j;
        for (int k = 0; k < 1L << i; ++k) {
            listGenerated[1L << i ^ k] = listGenerated[k] ^ j;
            generated[listGenerated[k] ^ j] = true;
        }
    }
    truthTable *result = malloc(sizeof(truthTable));
    result->dimension = dimension;
    result->elements = malloc(sizeof(size_t) * entries);
    memcpy(result->elements, listGenerated, sizeof(size_t) * entries);
    return result;
}

truthTable * getFunctionG(truthTable *functionF) {
    size_t n = functionF->dimension;
    truthTable *l1 = randomLinearPermutation(n);
    truthTable *l2 = randomLinearPermutation(n);
    truthTable *l = randomLinearFunction(n);

//    printTruthTable(l1);
//    printTruthTable(l2);
//    printTruthTable(l);

    truthTable *temp = composeFunctions(functionF, l2);
    truthTable *g = composeFunctions(l1, temp);
    addFunctionsTogether(g, l);

    destroyTruthTable(l1);
    destroyTruthTable(l2);
    destroyTruthTable(l);
    destroyTruthTable(temp);

//    printTruthTable(g);
    return g;
}
