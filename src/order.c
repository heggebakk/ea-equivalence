#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include "order.h"

char *filename;
char *writeFile = "order_test.txt";
FILE *fp;
size_t numPermutations;
size_t dimension;
size_t *basis;


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printGroupHelp();
        exit(0);
    }
    checkFlags(argc, argv);
    fp = fopen(writeFile, "w+");
    parsePermutationFile(filename);
    fclose(fp);
    return 0;
}

void printGroupHelp() {
    printf("Order\n");
    printf("Usage: order [order_options]\n");
    printf("Order options:\n");
    printf("\t-f Path to permutation\n");
    printf("\t-h  Help\n");
    printf("\t-w Path to where the results should be written to.\n");
    printf("\n");
}

size_t standardBasis() {
    basis = malloc(sizeof(size_t) * dimension);
    for (size_t i = 0; i < dimension; ++i) {
        basis[i] = (1L << i) - 1;
    }
}

void findGroups(TruthTable *src) {
    if (numPermutations == 1) {
        // If the number of permutation is 1, the permutation is the identity.
        return;
    }
    size_t counter = 1;
    TruthTable *dest = initTruthTable(dimension);
    memcpy(dest->elements, src->elements, sizeof(size_t) * 1L << dimension);
    while (true) {
        if (isIdentity(dest)) {
            fprintf(fp, "%zu\n", counter);
            break;
        }
        compose(src, dest);
        counter += 1;
        if (counter > numPermutations) {
            printf("Something's wrong!");
            exit(1);
        }
    }
    destroyTruthTable(dest);
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

void parsePermutationFile(char *input) {
    FILE *file = fopen(input, "r");
    if (file == NULL) {
        printf("Requested file, %s, not found.\n", input);
        fclose(file);
        exit(1);
    }

    // Fist line should be the dimension for the permutation
    fscanf(file, "%zu", &dimension);

    // Second line should be the total number of permutations
    fscanf(file, "%zu", &numPermutations);

    // Create the standard basis
    standardBasis();

    // Find the order for each permutation one by one.
    for (int i = 0; i < numPermutations; ++i) {
        // Save the permutation
        TruthTable *truthTable = initTruthTable(dimension);
        for (int j = 0; j < 1L << dimension; ++j) {
            fscanf(file, "%zu", &truthTable->elements[j]);
        }
        findGroups(truthTable);
        destroyTruthTable(truthTable);
    }
    fclose(file);
}

void checkFlags(int argc, char **argv) {
    for (int i = 0; i < argc; ++i) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'h':
                    // Help:
                    printGroupHelp();

                case 'f':
                    // File to read from
                    i++;
                    filename = argv[i];
                    continue;
                case 'w':
                    // Filename to write to
                    i++;
                    writeFile = argv[i];
                    continue;
            }
        }
    }
}
