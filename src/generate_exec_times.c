/**
 * @file generate_exec_times.c
 * @author Andres Barbosa, Milton Hernandez, Ivan Gallardo
 * @brief Funciones para la ejecution de tests
 */

#include "generate_exec_times.h"
#include "utilities.h"

/**
 * @brief compara merge sort clasico vs merge sort optimizado (threshold=MERGE_THRESHOLD)
 *        para observar la ganancia de la optimizacion sobre distintos tamanos.
 */
void run_experiment()
{
	int n = 0;
	Player* players;
	if ((players = load_players("build/db/players.csv", &n)) == NULL) {
		return;
	}

	// Rango de 2 hasta min(n, 50000) en NUM_STEPS pasos uniformes.
	const int n_min = 2;
	const int n_max = (n < 50000) ? n : 50000;
	const int num_points = NUM_STEPS;

	Player* classicPlayers   = malloc(n_max * sizeof(Player));
	Player* optimizedPlayers = malloc(n_max * sizeof(Player));

	if (!classicPlayers || !optimizedPlayers) {
		free(classicPlayers);
		free(optimizedPlayers);
		free(players);
		print_error(102, "players", NULL);
		return;
	}

	FILE *csv = fopen("build/db/experiment.csv", "w");
	if (csv == NULL) {
		free(classicPlayers);
		free(optimizedPlayers);
		free(players);
		print_error(101, "build/db/experiment.csv", NULL);
		return;
	}

	fprintf(csv, "N,Merge Sort Classic,Merge Sort Optimized\n");

	printf(PURPLE "╔══════════════════════════════════════════\n");

	for (int s = 0; s < num_points; s++) {
		int k = n_min + (int)((double)s / (num_points - 1) * (n_max - n_min));
		if (k < 1) k = 1;
		if (k > n) k = n;

		printf(PURPLE "║" MAGENTA "Procesando n =" WHITE " %8d" PURPLE "                   ║\n", k);
		printf(PURPLE "║" MAGENTA "Paso:" WHITE " %4d/%4d" PURPLE "                           ║\n", s + 1, num_points);

		clock_t start, end;

		// merge sort clasico
		double timeClassic = 0;
		for (int i = 0; i < NUM_TRIALS; i++) {
			memcpy(classicPlayers, players, (size_t)k * sizeof(Player));
			start = clock();
			merge_sort_classic(classicPlayers, 0, k - 1, compare_id);
			end = clock();
			timeClassic += (double)(end - start) / CLOCKS_PER_SEC;
		}
		timeClassic /= NUM_TRIALS;
		printf(PURPLE "║" MAG6 "\tMerge Classic:     " WHITE " %f" PURPLE "     ║\n", timeClassic);

		// merge sort optimizado con threshold = MERGE_THRESHOLD
		double timeOptimized = 0;
		for (int i = 0; i < NUM_TRIALS; i++) {
			memcpy(optimizedPlayers, players, (size_t)k * sizeof(Player));
			start = clock();
			merge_sort_optimized(optimizedPlayers, 0, k - 1, MERGE_THRESHOLD, compare_id);
			end = clock();
			timeOptimized += (double)(end - start) / CLOCKS_PER_SEC;
		}
		timeOptimized /= NUM_TRIALS;
		printf(PURPLE "║" MAG2 "\tMerge Optimizado:  " WHITE " %f" PURPLE "     ║\n", timeOptimized);

		fprintf(csv, "%d,%f,%f\n", k, timeClassic, timeOptimized);

		printf(PURPLE "╠══════════════════════════════════════════\n");
	}

	printf(PURPLE "║" LIGHT_GREEN "              CSV Data written" PURPLE "            ║\n");
	printf(PURPLE "╚══════════════════════════════════════════\n" RESET);
	fclose(csv);

	printf("\n" BG_GREEN "Data saved in build/db/experiment.csv" RESET "\n");

	free(players);
	free(classicPlayers);
	free(optimizedPlayers);
}

