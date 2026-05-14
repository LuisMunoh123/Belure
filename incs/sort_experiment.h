/**
 * @file sort_experiment.h
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Sorting experiment declarations.
 */

#ifndef SORT_EXPERIMENT_H
#define SORT_EXPERIMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "errors.h"
#include "experiment_config.h"
#include "generator.h"
#include "player.h"
#include "sorting.h"

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

void run_sort_experiment(void);

#endif
