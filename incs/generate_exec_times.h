/**
 * @file generate_exec_times.h
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Funciones para la ejecusion de tests (generate_exec_times.c)
 */
#ifndef EXEC_H
#define EXEC_H
#define NUM_TRIALS 20
#define NUM_STEPS 25
#include <stdio.h>
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
    double timeMergeOpt4;
    double timeMergeOpt8;
    double timeMergeOpt16;
    double timeMergeOpt32;
    double timeLinearSearch;
    double timeBinarySearch;
} ExecResults;

void run_experiment(void);

#endif
