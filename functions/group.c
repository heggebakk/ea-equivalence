#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include "group.h"
#include "../utils/linkedList.h"

char *filename;
size_t numPermutations;
size_t dimension;
size_t *basis;

void checkFlags(int argc, char **argv);

TtNode * parsePermutationFile(char *input);

void findGroups(TtNode *permutations);

void compose(TruthTable *src, TruthTable *dest);

bool isIdentity(TruthTable *dest);

size_t standardBasis();

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printHelp();
        exit(0);
    }
    checkFlags(argc, argv);
    TtNode *permutations = parsePermutationFile(filename);
    standardBasis(dimension);
    findGroups(permutations);
    destroyTtLinkedList(permutations);
    return 0;
}

size_t standardBasis() {
    basis = malloc(sizeof(size_t) * dimension);
    for (size_t i = 0; i < dimension; ++i) {
        basis[i] = (1L << i) - 1;
    }
}

void findGroups(TtNode *permutations) {
    for (int i = 0; i < numPermutations; ++i) {
        TruthTable *src = getNode(permutations, i);
        TruthTable *dest = initTruthTable(dimension);
        memcpy(dest->elements, src->elements, sizeof(size_t) * 1L << dimension);
        while (true) {
            if (isIdentity(dest)) {
                printTruthTable(dest);
                break;
            }
            compose(src, dest);
        }
        destroyTruthTable(dest);
    }
}

bool isIdentity(TruthTable *dest) {
    for (int i = 0; i < dimension; ++i) {
        if (dest->elements[(1L << i) - 1] != basis[i]) {
            return false;
        }
    }
    return true;
}

void compose(TruthTable *src, TruthTable *dest) {
    size_t *temp = malloc(sizeof(size_t) * 1L << dimension);
    for (int i = 0; i < 1L << dimension; ++i) {
        temp[i] = (*dest).elements[(*src).elements[i]];
    }
    free(dest->elements);
    dest->elements = temp;
}

TtNode * parsePermutationFile(char *input) {
    TtNode *permutations;
    FILE *file = fopen(input, "r");

    if (file == NULL) {
        printf("Requested file, %s, not found.\n", input);
        fclose(file);
        exit(1);
    }
    permutations = initTtNode();
    fscanf(file, "%zu", &dimension);
    fscanf(file, "%zu", &numPermutations);
    for (int i = 0; i < numPermutations; ++i) {
        TruthTable *truthTable = initTruthTable(dimension);
        for (int j = 0; j < 1L << dimension; ++j) {
            fscanf(file, "%zu", &truthTable->elements[j]);
        }
        addNode(permutations, truthTable);
        destroyTruthTable(truthTable);
    }
    fclose(file);
    return permutations;
}

void checkFlags(int argc, char **argv) {
    for (int i = 0; i < argc; ++i) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                // Help:
                case 'h':
                    printHelp();
                case 'f':
                    i++;
                    filename = argv[i];
                    continue;
            }
        }
    }
}

void printHelp() {
    printf("How to use the program:\n");
    printf("-f Input file \t Permutation file, first line is number of permutations, next n lines is the permutations.\n");
    printf("-d Dimension \t The dimension, which tells us the size of the tables.\n");
    printf("\n");
    printf("-h  Help\n");
}
