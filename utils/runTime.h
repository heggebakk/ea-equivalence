#ifndef EA_EQUIVALENCE_RUNTIME_H
#define EA_EQUIVALENCE_RUNTIME_H

#include <stdio.h>
#include "time.h"

/**
 * A struct for keeping all the run times for the entire program
 */
typedef struct {
    double total;
    double partition;
    double outerPermutation;
    double innerPermutation;
} RunTimes;

/**
 * Initialize a new empty struct of Run times
 * @return A empty RunTimes
 */
RunTimes *initRunTimes();

/**
 * Stop the time and return the run time
 * @param runTime The time to do the calculations on
 * @param startParsing The start time
 * @return The total run time
 */
double stopTime(double runTime, clock_t startParsing);

/**
 * Print all times to the console
 * @param runTimes the Run times to print
 */
void printTimes(RunTimes *runTimes);

/**
 * Write all times to a file
 * @param runTimes The run times to write
 * @param fp The file to write to
 */
void writeTimes(RunTimes *runTimes, FILE *fp);

/**
 * Destroy the run times
 * @param runTimes The run times to destroy
 */
void destroyRunTimes(RunTimes *runTimes);

#endif //EA_EQUIVALENCE_RUNTIME_H
