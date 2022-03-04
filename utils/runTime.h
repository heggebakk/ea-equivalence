#ifndef EA_EQUIVALENCE_RUNTIME_H
#define EA_EQUIVALENCE_RUNTIME_H

#include <stdio.h>
#include "time.h"

typedef struct {
    double total;
    double parsing;
    double partition;
    double outerPermutation;
    double innerPermutation;
} RunTimes;

RunTimes *initRunTimes();

double stopTime(double runTime, clock_t startParsing);

void printTimes(RunTimes *runTimes);

void writeTimes(RunTimes *runTimes, FILE *fp);

void destroyRunTimes(RunTimes *runTimes);

#endif //EA_EQUIVALENCE_RUNTIME_H
