/**
 * @file generate_exec_times.h
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Funciones para la ejecusion de tests (generate_exec_times.c)
 */
#ifndef EXEC_H
#define EXEC_H
#define NUM_TRIALS 100
#define NUM_STEPS 20
#define MERGE_THRESHOLD 48
#define LINEAR_REPEAT 1
#define SEARCH_REPEAT 10

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "errors.h"
#include "player.h"
#include "sorting.h"
#include "searching.h"
#include "generator.h"

typedef struct SortExecResults {
    int n;
    double timeSwapSort;
    double timeInsertionSort;
    double timeSelectionSort;
    double timeCocktailSort;
    double timeQuickSort;
    double timeMergeSort;
    double timeMergeSortOptimized;
} SortExecResults;

typedef struct SearchExecResults {
    int n;
    double timeLinearSearchWorst;
    double timeLinearSearchAverage;
    double timeBinarySearchWorst;
    double timeBinarySearchAverage;
    double timeBinarySearchRecursiveWorst;
    double timeBinarySearchRecursiveAverage;
    double timeExponentialSearchWorst;
    double timeExponentialSearchAverage;
    double timeInterpolationSearchWorst;
    double timeInterpolationSearchAverage;
    double timeBinarySearchRangeWorst;
    double timeBinarySearchRangeAverage;
} SearchExecResults;

void run_experiment(void);
void run_sort_experiment(void);
void run_search_experiment(void);

#endif
