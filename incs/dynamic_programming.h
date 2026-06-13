/**
 * @file dynamic_programming.h
 * @brief funciones de programacion dinamica para seleccionar un equipo con presupuesto.
 */

#ifndef DYNAMIC_PROGRAMMING_H
#define DYNAMIC_PROGRAMMING_H

#include "player.h"

typedef struct {
	Player *players;
	int selected_count;
	int total_cost;
	int total_score;
} DPResult;

DPResult dp_select_team_tabulation(Player players[], int n, int budget);
DPResult dp_select_team_memoization(Player players[], int n, int budget);
void free_dp_result(DPResult *result);

#endif // DYNAMIC_PROGRAMMING_H
