/**
 * @file dynamic_programming.c
 * @brief implementacion de programacion dinamica para seleccion con presupuesto.
 */

#include <stdlib.h>

#include "dynamic_programming.h"

static DPResult empty_result(void)
{
	DPResult result;
	result.players = NULL;
	result.selected_count = 0;
	result.total_cost = 0;
	result.total_score = 0;
	return result;
}

static int score_to_int(float score)
{
	return (int)(score * 10.0f + 0.5f);
}

static int max_int(int a, int b)
{
	if (a > b) {
		return a;
	}

	return b;
}

static int **create_dp_table(int rows, int cols)
{
	int **table = malloc(rows * sizeof(int *));

	if (table == NULL) {
		return NULL;
	}

	for (int i = 0; i < rows; i++) {
		table[i] = calloc(cols, sizeof(int));

		if (table[i] == NULL) {
			for (int j = 0; j < i; j++) {
				free(table[j]);
			}

			free(table);
			return NULL;
		}
	}

	return table;
}

static void free_dp_table(int **table, int rows)
{
	if (table == NULL) {
		return;
	}

	for (int i = 0; i < rows; i++) {
		free(table[i]);
	}

	free(table);
}

DPResult dp_select_team_tabulation(Player players[], int n, int budget)
{
	if (players == NULL || n <= 0 || budget <= 0) {
		return empty_result();
	}

	int **dp = create_dp_table(n + 1, budget + 1);
	Player *selected = malloc(n * sizeof(Player));

	if (dp == NULL || selected == NULL) {
		free_dp_table(dp, n + 1);
		free(selected);
		return empty_result();
	}

	for (int i = 1; i <= n; i++) {
		int cost = players[i - 1].costo;
		int value = score_to_int(players[i - 1].score);

		for (int b = 0; b <= budget; b++) {
			if (cost > b) {
				dp[i][b] = dp[i - 1][b];
			} else {
				int without_player = dp[i - 1][b];
				int with_player = value + dp[i - 1][b - cost];
				dp[i][b] = max_int(without_player, with_player);
			}
		}
	}

	DPResult result = empty_result();
	int current_budget = budget;

	for (int i = n; i > 0; i--) {
		if (dp[i][current_budget] != dp[i - 1][current_budget]) {
			selected[result.selected_count] = players[i - 1];
			result.selected_count++;
			result.total_cost += players[i - 1].costo;
			current_budget -= players[i - 1].costo;
		}
	}

	for (int left = 0, right = result.selected_count - 1; left < right; left++, right--) {
		Player temp = selected[left];
		selected[left] = selected[right];
		selected[right] = temp;
	}

	result.players = selected;
	result.total_score = dp[n][budget];

	free_dp_table(dp, n + 1);
	return result;
}

void free_dp_result(DPResult *result)
{
	if (result == NULL) {
		return;
	}

	free(result->players);
	result->players = NULL;
	result->selected_count = 0;
	result->total_cost = 0;
	result->total_score = 0;
}
