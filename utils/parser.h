#ifndef EA_EQUIVALENCE_PARSER_H
#define EA_EQUIVALENCE_PARSER_H
#include "truthTable.h"

truthTable * parseTruthTable(const char filename[64]);

void printTruthTable(truthTable *tt);

void writeTruthTable(truthTable *tt, FILE *fp, char *ttName);

#endif //EA_EQUIVALENCE_PARSER_H
