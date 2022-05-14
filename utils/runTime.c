#include <malloc.h>
#include <time.h>
#include "runTime.h"

RunTimes *initRunTimes() {
    RunTimes *newTime = malloc(sizeof(RunTimes));
    newTime->total = 0.0;
    newTime->partition = 0.0;
    newTime->outerPermutation = 0.0;
    newTime->innerPermutation = 0.0;
    return newTime;
}

double stopTime(double runTime, clock_t startParsing) {
    runTime += (double) (clock() - startParsing) / CLOCKS_PER_SEC;
    return runTime;
}

void printTimes(RunTimes *runTimes) {
    printf("Time spent partitioning: %f \n", runTimes->partition);
    printf("Time spent outer permutation: %f \n", runTimes->outerPermutation);
    printf("Time spent inner permutation: %f \n", runTimes->innerPermutation);
    printf("Total time spent: %f \n", runTimes->total);
}

void writeTimes(RunTimes *runTimes, FILE *fp) {
    fprintf(fp, "Time spent partitioning: %f \n", runTimes->partition);
    fprintf(fp, "Time spent outer permutation: %f \n", runTimes->outerPermutation);
    fprintf(fp, "Time spent inner permutation: %f \n", runTimes->innerPermutation);
    fprintf(fp, "Total time spent: %f \n", runTimes->total);
}

void destroyRunTimes(RunTimes *runTimes) {
    free(runTimes);
}
