//
// Created by marie on 11/12/21.
//

#include "compareTruthTable.h"

bool compareTruthTable(truthTable f, truthTable g) {
    if (f.dimension != g.dimension) return false;
    for (size_t i = 0; i < 1L << f.dimension; ++i) {
        if (f.elements[i] != g.elements[i]) return false;
    }
    return true;
}
