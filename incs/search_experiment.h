/**
 * @file search_experiment.h
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Search experiment declarations.
 */

#ifndef SEARCH_EXPERIMENT_H
#define SEARCH_EXPERIMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "errors.h"
#include "experiment_config.h"
#include "generator.h"
#include "player.h"
#include "searching.h"
#include "sorting.h"

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

void run_search_experiment(void);

#endif
