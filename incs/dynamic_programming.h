/**
 * @file dynamic_programming.h
 * @brief funciones de programacion dinamica para seleccionar un equipo con presupuesto.
 */

#ifndef DYNAMIC_PROGRAMMING_H
#define DYNAMIC_PROGRAMMING_H

#include "player.h"

typedef enum {
	DP_INVALID = 0,
	DP_TABULATION,
	DP_MEMOIZATION
} DPAlgorithm;

typedef struct {
	Player *players;     // deportistas seleccionados
	int selected_count;  // cantidad de deportistas seleccionados
	int total_cost;      // suma de costos del equipo
	int total_score;     // suma de puntajes multiplicada por 10
} DPResult;

DPResult dp_select_team_tabulation(Player players[], int n, int budget);
DPResult dp_select_team_memoization(Player players[], int n, int budget);
void free_dp_result(DPResult *result);

#endif // DYNAMIC_PROGRAMMING_H
