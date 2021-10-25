#include <malloc.h>
#include <stdbool.h>
#include "innerPermutation.h"
#include "../utils/linkedList.h"
#include "../utils/freeMemory.h"

void innerPermutation(truthTable f, truthTable g, size_t *basis) {
    struct Node **domains = calloc(sizeof(struct Node*), f.dimension); // A list of Linked Lists with the restricted domains

    for (int i = 0; i < f.dimension; ++i) {
        bool *map = computeSetOfTs(f, g, basis[i]);
        domains[i] = computeDomain(map, f);
        free(map);
    }
    for (int i = 0; i < f.dimension; ++i) {
        displayLinkedList(domains[i]);
    }

    reconstructInnerPermutation(domains, f.dimension);

    for (size_t i = 0; i < f.dimension; ++i) {
        freeLinkedList(domains[i]);
    }
    free(domains);
}

bool * computeSetOfTs(truthTable f, truthTable g, size_t x) {
    size_t n = f.dimension;
    bool *map = calloc(sizeof(bool), 1L << n);

    for (size_t y = 0; y < 1L << n; ++y) {
        size_t t = g.elements[x] ^ g.elements[y] ^ g.elements[x ^ y];
        map[t] = true;
    }
    return map;
}

/**
 * Compute the restricted domain for the given list of T's.
 * The domain is represented with a linked list.
 * @param listOfTs A set of T's that we want to compute the domain for
 * @param f A function F
 * @return The restricted domain represented as a linked list.
 */
struct Node * computeDomain(const bool *listOfTs, truthTable f) {
    size_t n = f.dimension;
    bool *domain = calloc(sizeof(bool), 1L << n);
    for (size_t i = 0; i < 1L << n; ++i) {
        domain[i] = true;
    }
    for (int t = 0; t < 1L << n; ++t) {
        if (listOfTs[t]) {
            bool *tempSet = calloc(sizeof(bool), 1L << n);
            for (size_t x = 0; x < 1L << n; ++x) {
                for (size_t y = 0; y < 1L << n; ++y) {
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

    // Restricted domain represented as a Linked List
    struct Node *head = NULL;
    struct Node *tail = NULL;
    for (size_t i = 0; i < 1L << n; ++i) {
        if (domain[i]) {
            // Add a new node to the linked list
            struct Node *newNode = (struct Node*) malloc(sizeof(struct Node));
            newNode->data = i;
            newNode->next = NULL;

            // Check if the linked list is empty
            if (head == NULL) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
    }
    free(domain);
    return head;
}

/**
 *
 * @param domains
 * @param dimension
 */
void reconstructInnerPermutation(struct Node **domains, size_t dimension) {
    size_t *values = calloc(sizeof(size_t), dimension);
    dfs(domains, dimension, 0, values);
    free(values);
}

/**
 * A depth first search over a array containing linked lists.
 */
void dfs(struct Node **domains, size_t dimension, size_t k, size_t *values) {
    if (k >= dimension) {
        reconstructTruthTable(values, dimension);
        return;
    }
    struct Node *current = domains[k];
    while (current != NULL) {
        values[k] = current->data;
        dfs(domains, dimension, k + 1, values);
        current = current->next;
    }
}

/**
 * Reconstruction of a truth table
 * @param basisValues A standard basisValues, powers of 2
 * @param dimension Dimension of the Function
 */
void reconstructTruthTable(const size_t *basisValues, size_t dimension) {
    size_t *truthTable = calloc(sizeof(size_t), 1L << dimension);

    for (size_t coordinate = 0; coordinate < 1L << dimension; ++coordinate) {
        size_t result = 0;
        for (size_t i = 0; i < dimension; ++i) {
            if ((1L << i) & coordinate) {
                result = result ^ basisValues[i];
            }
        }
        truthTable[coordinate] = result;
    }
    free(truthTable);
}

/**
 * Compose function F with function G
 * @param f
 * @param g
 */
size_t * composeFunctions(const size_t *f, const size_t *g, size_t dimension) {
    size_t *result = calloc(sizeof(size_t), 1L << dimension);
    for (size_t x = 0; x < 1L << dimension; ++x) {
        result[x] = f[g[x]];
    }
    return result;
}
