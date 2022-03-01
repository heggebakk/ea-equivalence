#include <stdlib.h>
#include "freeMemory.h"
#include "parser.h"

void freeTruthTable(truthTable *tt) {
    free(tt->elements);
    free(tt);
}

void freeTtWt(walshTransform *wt) {
    for (size_t i = 0; i < 1L << wt->dimension; ++i) {
        free(wt->elements[i]);
    }
    free(wt->elements);
    free(wt);
}

void freePartition(partitions *p) {
    for (size_t i = 0; i < p->numberOfClasses; ++i) {
        free(p->classes[i]);
    }
    free(p->classSizes);
    free(p->multiplicities);
    free(p->classes);
    free(p);
}
