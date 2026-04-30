/**
 * @file generate_exec_times.h
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Funciones para la ejecusion de tests (generate_exec_times.c)
 */
#ifndef EXEC_H
#define EXEC_H
#define NUM_TRIALS 1000
#define NUM_STEPS 25
#include <stdio.h>
#include <time.h>

#include "errors.h"
#include "player.h"
#include "sorting.h"
#include "searching.h"
#include "generator.h"

/* typedef struct ExecResults {
    int n;
    double timeSwapSort;
    double timeInsertionSort;
    double timeSelectionSort;
    double timeCocktailSort;
    double timeLinearSearch;
    double timeBinarySearch;
} ExecResults; */

typedef struct ExecResults {
    int n;
    double timeLeftPivot;
    double timeRightPivot;
    double timeMedianPivot;
    double timeRandomPivot;
} ExecResults;

void run_experiment(void);

#endif