#ifndef BOOLEAN_FUNCTION_FILEPARSER_H
#define BOOLEAN_FUNCTION_FILEPARSER_H

#include <stddef.h>

struct vbfTruthTable {
    size_t dimension;
    size_t elements;
    int * array;
};
struct vbfTruthTable parseTruthTable(const char filename[64]);
#endif //BOOLEAN_FUNCTION_FILEPARSER_H

