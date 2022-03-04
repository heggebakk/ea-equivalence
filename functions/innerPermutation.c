#include <malloc.h>
#include <stdbool.h>
#include "innerPermutation.h"

bool innerPermutation(truthTable *f, truthTable *g, const size_t *basis, truthTable *l2, truthTable **lPrime) {
    Node **restrictedDomains = calloc(sizeof(Node **), f->dimension); // A list of Linked Lists

    for (size_t i = 0; i < f->dimension; ++i) {
        bool *map = computeSetOfTs(g, basis[i]);
        restrictedDomains[i] = computeDomain(map, f);
        free(map);
    }
    size_t *values = malloc(sizeof(size_t) * f->dimension);
    bool result = dfs(restrictedDomains, 0, values, f, g, l2, lPrime);
    free(values);

    for (size_t i = 0; i < f->dimension; ++i) {
        freeLinkedList(restrictedDomains[i]);
    }
    free(restrictedDomains);
    return result;
}

bool *computeSetOfTs(truthTable *f, size_t x) {
    size_t n = f->dimension;
    bool *map = calloc(sizeof(bool), 1L << n);

    for (size_t y = 0; y < 1L << n; ++y) {
        size_t t = f->elements[x] ^ f->elements[y] ^ f->elements[x ^ y];
        map[t] = true;
    }
    return map;
}

Node *computeDomain(const bool *listOfTs, truthTable *f) {
    size_t n = f->dimension;
    bool *domain = calloc(sizeof(bool), 1L << n);
    for (size_t i = 0; i < 1L << n; ++i) {
        domain[i] = true;
    }
    for (size_t t = 0; t < 1L << n; ++t) {
        if (listOfTs[t]) {
            bool *tempSet = calloc(sizeof(bool), 1L << n);
            for (size_t x = 0; x < 1L << n; ++x) {
                for (size_t y = 0; y < 1L << n; ++y) {
                    if (t == (f->elements[x] ^ f->elements[y] ^ f->elements[x ^ y])) {
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

    // Restricted domain represented as a Linked List
    Node *head = initLinkedList();
    for (size_t i = 0; i < 1L << n; ++i) {
        if (domain[i]) {
            // Add a new node to the linked list
            addToLinkedList(head, i);
        }
    }
    free(domain);
    return head;
}

bool dfs(Node **domains, size_t k, size_t *values, truthTable *f, truthTable *g, truthTable *l2, truthTable **lPrime) {
    if (k >= f->dimension) {
        reconstructTruthTable(values, l2);
        *lPrime = composeFunctions(f, l2);
        addFunctionsTogether(*lPrime, g);
        if (isLinear(*lPrime)) {
            return true;
        }
        destroyTruthTable(*lPrime);
        return false;
    }
    Node *current = domains[k];
    while (current != NULL) {
        values[k] = current->data;
        bool linear = dfs(domains, k + 1, values, f, g, l2, lPrime);
        if (linear) return true;
        current = (Node *) current->next;
    }
    return false;
}

void reconstructTruthTable(const size_t *basisValues, truthTable *l2) {
    for (size_t coordinate = 0; coordinate < 1L << l2->dimension; ++coordinate) {
        size_t result = 0;
        for (size_t i = 0; i < l2->dimension; ++i) {
            if (1L << i & coordinate) {
                result ^= basisValues[i];
            }
        }
        l2->elements[coordinate] = result;
    }
}

truthTable *composeFunctions(truthTable *f, truthTable *g) {
    truthTable *result = malloc(sizeof(truthTable));
    result->dimension = f->dimension;
    result->elements = calloc(sizeof(size_t), 1L << f->dimension);
    for (size_t x = 0; x < 1L << f->dimension; ++x) {
        result->elements[x] = f->elements[g->elements[x]];
    }
    return result;
}

void addFunctionsTogether(truthTable *to, truthTable *from) {
    for (size_t i = 0; i < 1L << to->dimension; ++i) {
        to->elements[i] ^= from->elements[i];
    }
}

bool isLinear(truthTable *f) {
    for (size_t a = 1; a < 1L << f->dimension; ++a) {
        for (size_t b = a + 1; b < 1L << f->dimension; ++b) {
            if (b > (a ^ b)) continue;
            size_t result = f->elements[0] ^ f->elements[a] ^ f->elements[b] ^ f->elements[a ^ b];
            if (result != 0) return false;
        }
    }
    return true;
}
