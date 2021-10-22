#include <malloc.h>
#include <stdbool.h>
#include "innerPermutation.h"
#include "../utils/linkedList.h"
#include "../utils/freeMemory.h"

//void reconstructInnerPermutation(size_t *domain, truthTable f, truthTable g);
//
//void recursiveLoop(size_t *domain, size_t *basis, size_t numOfBasis, size_t n);

void innerPermutation(truthTable f, truthTable g, size_t *basis) {
    struct Node *domains = calloc(sizeof(struct Node), f.dimension); // A list of Linked Lists with the restricted domains

    for (int i = 0; i < f.dimension; ++i) {
        bool *map = computeSetOfTs(f, g, basis[i]);
        domains[i] = *computeDomain(map, f);
        free(map);
    }
//    reconstructInnerPermutation(domain, f, g);
    // TODO: Free memory not working
    for (size_t i = 0; i < f.dimension; ++i) {
        freeLinkedLists(&domains[i]);
    }
}

bool * computeSetOfTs(truthTable f, truthTable g, size_t x) {
    size_t n = f.dimension;
    bool *map = calloc(sizeof(bool), 1L << n);

    for (size_t y = 0; y < 1L << n; ++y) {
        size_t t = g.elements[x] ^ g.elements[y] ^ g.elements[x ^ y];
        map[t] = true;
    }
    printf("Map of all t's: ");
    for (int i = 0; i < 1L << n; ++i) {
        printf("%d ", map[i]);
    }
    printf("\n");
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
    bool *domain = malloc(sizeof(bool) * 1L << n);
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
    printf("Restricted domain: ");
    for (int i = 0; i < 1L << n; ++i) {
        printf("%d ", domain[i]);
    }
    printf("\n");

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

    displayLinkedList(head);
    countNodes(head);

    return head;
}

// dfs
// b1 basis, d1 domain 1, d2 domain 2
//void reconstructInnerPermutation(size_t *domain, size_t *basis, size_t numOfBasis) {
//    recursiveLoop(domain, basis, numOfBasis, numOfBasis);
//}

// array med pointer for [ lagre alle verdier v
//void recursiveLoop(size_t *domain, size_t *basis, size_t numOfBasis, size_t n, size_t i) {
//    if (i >= n) {
//         Do something
//         print alle verdier vi har gitt
//        return;
//    }
    // for loop for alle verdier i domain for verdi i
//    recursiveLoop(domain, basis, numOfBasis, n, i + 1);
//}
