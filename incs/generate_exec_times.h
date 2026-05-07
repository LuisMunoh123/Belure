/**
 * @file generate_exec_times.h
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Funciones para la ejecusion de tests (generate_exec_times.c)
 */
#ifndef EXEC_H
#define EXEC_H
#define NUM_TRIALS 100
#define NUM_STEPS 100
#include <stdio.h>
#include <time.h>

#include "errors.h"
#include "player.h"
#include "sorting.h"
#include "searching.h"
#include "generator.h"

typedef struct ExecResults {
    int n;
    double timeInsertionSort;
    double timeMergeClassic;
} ExecResults;

void run_experiment(void);

#endif
