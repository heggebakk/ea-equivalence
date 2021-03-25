#include "utils/fileParser.h"
#include "functions/computationOfMultiplicities.h"

typedef struct vbfTruthTable truthTable;

int main(void) {
    char *filename;
    filename = "/home/marie/MasterUiB/ea-equivalence/src/resources/TT_library/dim6/q_6_3.tt";
    truthTable tt = parseTruthTable(filename);
    computationOfMultiplicities(&tt, 4);
    return 0;
}