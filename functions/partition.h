#ifndef EA_EQUIVALENCE_PARTITION_H
#define EA_EQUIVALENCE_PARTITION_H
#include "../utils/structs.h"

partitions partitionFunction(truthTable *function, size_t k, size_t t);

void printPartitionInfo(partitions p);

#endif //EA_EQUIVALENCE_PARTITION_H
