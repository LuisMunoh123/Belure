/**
 * @file generate_exec_times.h
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Funciones para la ejecusion de tests (generate_exec_times.c)
 */
#ifndef EXEC_H
#define EXEC_H
#define NUM_TRIALS 100
#define NUM_STEPS 40
#define MERGE_THRESHOLD 48

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "errors.h"
#include "player.h"
#include "sorting.h"
#include "searching.h"
#include "generator.h"

typedef struct ExecResults {
    int n;
    double timeSwapSort;
    double timeInsertionSort;
    double timeSelectionSort;
    double timeCocktailSort;
    double timeQuickSort;
    double timeMergeClassic;
    double timeLinearSearch;
    double timeBinarySearch;
} ExecResults;

void run_experiment(void);
void run_merge_threshold_experiment(void);

#endif
