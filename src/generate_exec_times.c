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

	const int thresholds[] = {16, 24, 32, 40, 48, 56, 64};
	const int num_thresholds = 7;

	Player* buf = malloc((size_t)n_max * sizeof(Player));
	if (!buf) {
		free(players);
		print_error(102, "players", NULL);
		return;
	}

	FILE *csv = fopen("build/db/experiment.csv", "w");
	if (csv == NULL) {
		free(buf);
		free(players);
		print_error(101, "build/db/experiment.csv", NULL);
		return;
	}

	// Cabecera CSV
	fprintf(csv, "N,Merge Sort Classic");
	for (int t = 0; t < num_thresholds; t++) {
		fprintf(csv, ",Merge Sort Optimizado %d", thresholds[t]);
	}
	fprintf(csv, "\n");

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
			memcpy(buf, players, (size_t)k * sizeof(Player));
			start = clock();
			merge_sort_classic(buf, 0, k - 1, compare_id);
			end = clock();
			timeClassic += (double)(end - start) / CLOCKS_PER_SEC;
		}
		timeClassic /= NUM_TRIALS;
		printf(PURPLE "║" MAG6 "\tMerge Classic:       " WHITE " %f" PURPLE "     ║\n", timeClassic);

		fprintf(csv, "%d,%f", k, timeClassic);

		// merge sort optimizado para cada threshold
		for (int t = 0; t < num_thresholds; t++) {
			double timeOpt = 0;
			for (int i = 0; i < NUM_TRIALS; i++) {
				memcpy(buf, players, (size_t)k * sizeof(Player));
				start = clock();
				merge_sort_optimized(buf, 0, k - 1, thresholds[t], compare_id);
				end = clock();
				timeOpt += (double)(end - start) / CLOCKS_PER_SEC;
			}
			timeOpt /= NUM_TRIALS;
			printf(PURPLE "║" MAG2 "\tMerge Opt %-3d:       " WHITE " %f" PURPLE "     ║\n", thresholds[t], timeOpt);
			fprintf(csv, ",%f", timeOpt);
		}

		fprintf(csv, "\n");
		printf(PURPLE "╠══════════════════════════════════════════\n");
	}

	printf(PURPLE "║" LIGHT_GREEN "              CSV Data written" PURPLE "            ║\n");
	printf(PURPLE "╚══════════════════════════════════════════\n" RESET);
	fclose(csv);

	printf("\n" BG_GREEN "Data saved in build/db/experiment.csv" RESET "\n");

	free(players);
	free(buf);
}

/**
 * @brief Experimento para encontrar el threshold optimo de merge_sort_optimized.
 *        Usa n = 10000, prueba thresholds {16, 24, 32, 40, 48, 64} y repite
 *        NUM_TRIALS veces cada uno. Guarda resultados en
 *        build/db/merge_threshold_experiment.csv
 */
void run_merge_threshold_experiment(void)
{
	const int N = 10000;
	int n = 0;

	Player *players = load_players("build/db/players.csv", &n);
	if (players == NULL) {
		return;
	}

	if (n < N) {
		printf(LIGHT_RED "Error: el CSV tiene solo %d jugadores, se necesitan %d.\n" RESET, n, N);
		printf(LIGHT_RED "Genera primero: ./build/conquer.out -g %d -t shuffled\n" RESET, N);
		free(players);
		return;
	}

	Player *copy = malloc(N * sizeof(Player));
	if (copy == NULL) {
		free(players);
		print_error(102, "copy", NULL);
		return;
	}

	FILE *csv = fopen("build/db/merge_threshold_experiment.csv", "w");
	if (csv == NULL) {
		free(copy);
		free(players);
		print_error(101, "build/db/merge_threshold_experiment.csv", NULL);
		return;
	}

	fprintf(csv, "threshold,time\n");

	int thresholds[] = {36, 40, 44, 48};
	const int num_thresholds = (int)(sizeof(thresholds) / sizeof(thresholds[0]));

	printf(PURPLE "╔══════════════════════════════════════════════╗\n");
	printf(PURPLE "║" MAGENTA "   Experimento: threshold optimo merge sort   " PURPLE "║\n");
	printf(PURPLE "║" WHITE "   n = %-6d   trials = %-6d              " PURPLE "║\n", N, NUM_TRIALS);
	printf(PURPLE "╠══════════════════════════════════════════════╣\n");
	printf(PURPLE "║" WHITE " %-10s  %-28s " PURPLE "║\n", "threshold", "tiempo promedio (s)");
	printf(PURPLE "╠══════════════════════════════════════════════╣\n");

	int best_threshold = thresholds[0];
	double best_time = -1.0;

	for (int t = 0; t < num_thresholds; t++) {
		int thr = thresholds[t];
		double total_time = 0.0;

		for (int i = 0; i < NUM_TRIALS; i++) {
			memcpy(copy, players, N * sizeof(Player));
			clock_t start = clock();
			merge_sort_optimized(copy, 0, N - 1, thr, compare_id);
			clock_t end = clock();
			total_time += (double)(end - start) / CLOCKS_PER_SEC;
		}

		double avg_time = total_time / NUM_TRIALS;

		printf(PURPLE "║" MAGENTA " %-10d" WHITE "  %-28f " PURPLE "║\n", thr, avg_time);
		fprintf(csv, "%d,%f\n", thr, avg_time);

		if (best_time < 0.0 || avg_time < best_time) {
			best_time = avg_time;
			best_threshold = thr;
		}
	}

	printf(PURPLE "╠══════════════════════════════════════════════╣\n");
	printf(PURPLE "║" LIGHT_GREEN " Mejor threshold: %-4d  tiempo: %-14f " PURPLE "║\n", best_threshold, best_time);
	printf(PURPLE "╚══════════════════════════════════════════════╝\n" RESET);

	fclose(csv);
	printf("\n" BG_GREEN "Resultados guardados en build/db/merge_threshold_experiment.csv" RESET "\n");

	free(copy);
	free(players);
}

