#include <malloc.h>
#include <stdbool.h>
#include "innerPermutation.h"
#include "../utils/linkedList.h"
#include "../utils/freeMemory.h"
#include "../utils/parser.h"

bool innerPermutation(truthTable *f, truthTable *g, const size_t *basis, truthTable *l2, truthTable *lPrime) {
    struct Node **restrictedDomains = calloc(sizeof(struct Node), f->dimension); // A list of Linked Lists

    for (size_t i = 0; i < f->dimension; ++i) {
        bool *map = computeSetOfTs(f, g, basis[i]);
        restrictedDomains[i] = computeDomain(map, f);
        free(map);
    }

    bool result = reconstructInnerPermutation(restrictedDomains, f, g, l2, lPrime);

    for (size_t i = 0; i < f->dimension; ++i) {
        freeLinkedList(restrictedDomains[i]);
    }
    free(restrictedDomains);
    return result;
}

bool * computeSetOfTs(truthTable *f, truthTable *g, size_t x) {
    size_t n = f->dimension;
    bool *map = calloc(sizeof(bool), 1L << n);

    for (size_t y = 0; y < 1L << n; ++y) {
        size_t t = g->elements[x] ^ g->elements[y] ^ g->elements[x ^ y];
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
struct Node * computeDomain(const bool *listOfTs, truthTable *f) {
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
bool
reconstructInnerPermutation(struct Node **domains, truthTable *f, truthTable *g, truthTable *l2, truthTable *lPrime) {
    size_t *values = calloc(sizeof(size_t), f->dimension);
    bool result = dfs(domains, f->dimension, 0, values, f, g, l2, lPrime);
    free(values);
    return result;
}

/**
 * A depth first search over a array containing linked lists.
  * @param domains A array containing linked list of domains
  * @param dimension The dimension of the function
  * @param k
  * @param values
  * @return True if the reconstructed truth table is linear, false otherwise.
  */
bool
dfs(struct Node **domains, size_t dimension, size_t k, size_t *values, truthTable *f, truthTable *g, truthTable *l2,
    truthTable *lPrime) {
    if (k >= dimension) {
        reconstructTruthTable(values, l2);
        if(l2->elements[1] == 10 && l2->elements[2] == 44 && l2->elements[3] == 38 && l2->elements[63] == 21) {
            printf("FOund it!!!");
            printTruthTable(*l2);
        }
        truthTable composed = composeFunctions(f, l2); // UPDATE TO lPrime
        addFunctionsTogether(&composed, g);
        if(isLinear(&composed)) {
            freeTruthTable(composed);
            return true;
        }
        freeTruthTable(composed);
    }
    struct Node *current = domains[k];
    while (current != NULL) {
        values[k] = current->data;
        bool linear = dfs(domains, dimension, k + 1, values, f, g, l2, NULL);
        if (linear) return true;
        current = current->next;
    }
    return false;
}

/**
 * Reconstruction of a truth table
 * @param basisValues A standard basisValues, powers of 2
 * @param dimension Dimension of the Function
 */
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

/**
 * Compose function F with function G
 * @param f Function that is f be composed
 * @param g Function that is composed with
 */
truthTable composeFunctions(truthTable *f, truthTable *g) {
    truthTable result;
    result.dimension = f->dimension;
    result.elements = calloc(sizeof(size_t), 1L << f->dimension);
    for (size_t x = 0; x < 1L << f->dimension; ++x) {
        result.elements[x] = f->elements[g->elements[x]];
    }
    return result;
}

/**
 * Add function F with function G
  * @param to
  * @param from
  * @param dimension
  * @return
  */
void addFunctionsTogether(truthTable *to, truthTable *from) {
    for (size_t i = 0; i < 1L << to->dimension; ++i) {
        to->elements[i] ^= from->elements[i];
    }
}

/**
 * Check if a function is linear or not
 * @param f The function to check
 * @param dimension The dimension of the function
 * @return True if the function is linear, false otherwise
 */
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
