#include "utils/fileParser.h"

typedef struct vbfTruthTable truthTable;

int main(void) {
    char *filename;
    filename = "/home/marie/MasterUiB/ea-equivalence/src/resources/TT_library/dim6/q_6_1.tt";
    parseTruthTable(filename);
    return 0;
}