/**
 * @file generate_exec_times.h
 * @author Andres Barbosa, Milton Hernandez, Ivan Gallardo
 * @brief Funciones para la ejecusion de tests (generate_exec_times.c)
 */
#ifndef EXEC_H
#define EXEC_H
#define NUM_TRIALS 500
#define NUM_STEPS 20
// piernafloo *crack*
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
    double timeLinearSearch;
    double timeBinarySearch;
} ExecResults;

// Pero bueno, finalizo esta carta que te escribo desde mi hoja !A4!.
// main.c

void run_experiment(void);

#endif