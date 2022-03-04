#include <stdlib.h>
#include "truthTable.h"

void destroyTruthTable(truthTable *tt) {
    free(tt->elements);
    free(tt);
}