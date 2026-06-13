/**
 * @file search_experiment.c
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Functions for running search experiments.
 */

#include "search_experiment.h"
#include "utilities.h"

/**
 * @brief Finds an ID that does not exist within the current sorted prefix.
 *
 * @param players Array sorted by ID.
 * @param n Number of valid elements to inspect.
 * @return int Missing ID inside or just after the inspected range.
 */
static int pick_missing_id(Player players[], int n)
{
	if (n <= 0) return -1;

	for (int i = 0; i < n - 1; i++) {
		if (players[i + 1].id - players[i].id > 1) {
			return players[i].id + 1;
		}
	}

	return players[n - 1].id + 1;
}

/**
 * @brief Finds a score that does not exist within the current sorted prefix.
 *
 * @param players Array sorted by score.
 * @param n Number of valid elements to inspect.
 * @return float Missing score inside or just after the inspected range.
 */
static float pick_missing_score(Player players[], int n)
{
	if (n <= 0) return 0.0f;

	for (int i = 0; i < n - 1; i++) {
		int current_score = (int)(players[i].score * 10.0f + 0.5f);
		int next_score = (int)(players[i + 1].score * 10.0f + 0.5f);

		if (next_score - current_score > 1) {
			return (current_score + 1) / 10.0f;
		}
	}

	return players[n - 1].score + 0.1f;
}

void run_search_experiment()
{
	int n = 0;
	Player* players;
	if ((players = load_players("build/db/players.csv", &n)) == NULL) {
		return;
	}

	printf("Arreglo de jugadores cargado\n");

	const int num_points = NUM_STEPS;
	SearchExecResults resultados[num_points];
	int struct_idx = 0;

	Player* linearSearchPlayers = malloc(n * sizeof(Player));
	Player* idSortedPlayers = malloc(n * sizeof(Player));
	Player* scoreRangePlayers = malloc(n * sizeof(Player));

	if (!linearSearchPlayers || !idSortedPlayers || !scoreRangePlayers) {
		free(linearSearchPlayers);
		free(idSortedPlayers);
		free(scoreRangePlayers);
		free(players);
		print_error(102, "players", NULL);
		return;
	}

	memcpy(linearSearchPlayers, players, n * sizeof(Player));
	memcpy(idSortedPlayers, players, n * sizeof(Player));
	memcpy(scoreRangePlayers, players, n * sizeof(Player));

	quick_sort(idSortedPlayers, 0, n - 1, compare_id);
	quick_sort(scoreRangePlayers, 0, n - 1, compare_score);

	FILE *csv = fopen("build/db/experiment.csv", "w");
	if (csv == NULL) {
		free(players);
		free(linearSearchPlayers);
		free(idSortedPlayers);
		free(scoreRangePlayers);
		print_error(101, "build/db/experiment.csv", NULL);
		return;
	}

	fprintf(csv, "N,Linear Worst,Linear Average,Binary Worst,Binary Average,Binary Recursive Worst,Binary Recursive Average,Exponential Worst,Exponential Average,Interpolation Worst,Interpolation Average,Range Score Worst,Range Score Average\n");

	printf(PURPLE "╔══════════════════════════════════════════╗\n");

	for (int s = 0; s < num_points; s++) {
		int k = (num_points == 1) ? n : 1 + (s * (n - 1)) / (num_points - 1);
		clock_t start, end;
		int missing_id;
		float missing_score;
		Player idWorstTarget = {0, "", "", 0.0f, 0, 0};
		Player scoreWorstTarget = {0, "", "", 0.0f, 0, 0};
		double timeLinearSearchWorst = 0.0;
		double timeLinearSearchAverage = 0.0;
		double timeBinarySearchWorst = 0.0;
		double timeBinarySearchAverage = 0.0;
		double timeBinarySearchRecursiveWorst = 0.0;
		double timeBinarySearchRecursiveAverage = 0.0;
		double timeExponentialSearchWorst = 0.0;
		double timeExponentialSearchAverage = 0.0;
		double timeInterpolationSearchWorst = 0.0;
		double timeInterpolationSearchAverage = 0.0;
		double timeBinarySearchRangeWorst = 0.0;
		double timeBinarySearchRangeAverage = 0.0;

		printf(PURPLE "║" MAGENTA "Procesando n =" WHITE" %8d" PURPLE "                   ║\n", k);
		printf(PURPLE "║" MAGENTA "Paso:" WHITE" %4d/%4d" PURPLE "                           ║\n", s + 1, num_points);

		missing_id = pick_missing_id(idSortedPlayers, k);
		missing_score = pick_missing_score(scoreRangePlayers, k);
		idWorstTarget.id = missing_id;
		scoreWorstTarget.score = missing_score;

		for (int i = 0; i < NUM_TRIALS; i++) {
			Player averageIdTarget = {idSortedPlayers[rand() % k].id, "", "", 0.0f, 0, 0};
			Player averageScoreTarget = {0, "", "", scoreRangePlayers[rand() % k].score, 0, 0};
			int averageInterpolationId = idSortedPlayers[rand() % k].id;
			int first = -1;
			int last = -1;

			start = clock();
			for (int rep = 0; rep < LINEAR_REPEAT; rep++) {
				linear_search(linearSearchPlayers, k, &idWorstTarget, compare_id);
			}
			end = clock();
			timeLinearSearchWorst += ((double)(end - start) / CLOCKS_PER_SEC) / LINEAR_REPEAT;

			start = clock();
			for (int rep = 0; rep < LINEAR_REPEAT; rep++) {
				linear_search(linearSearchPlayers, k, &averageIdTarget, compare_id);
			}
			end = clock();
			timeLinearSearchAverage += ((double)(end - start) / CLOCKS_PER_SEC) / LINEAR_REPEAT;

			start = clock();
			for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
				binary_search_iterative(idSortedPlayers, 0, k - 1, &idWorstTarget, compare_id);
			}
			end = clock();
			timeBinarySearchWorst += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;

			start = clock();
			for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
				binary_search_iterative(idSortedPlayers, 0, k - 1, &averageIdTarget, compare_id);
			}
			end = clock();
			timeBinarySearchAverage += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;

			start = clock();
			for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
				binary_search_recursive(idSortedPlayers, 0, k - 1, &idWorstTarget, compare_id);
			}
			end = clock();
			timeBinarySearchRecursiveWorst += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;

			start = clock();
			for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
				binary_search_recursive(idSortedPlayers, 0, k - 1, &averageIdTarget, compare_id);
			}
			end = clock();
			timeBinarySearchRecursiveAverage += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;

			start = clock();
			for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
				exponential_search(idSortedPlayers, k, &idWorstTarget, compare_id);
			}
			end = clock();
			timeExponentialSearchWorst += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;

			start = clock();
			for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
				exponential_search(idSortedPlayers, k, &averageIdTarget, compare_id);
			}
			end = clock();
			timeExponentialSearchAverage += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;

			start = clock();
			for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
				interpolation_search(idSortedPlayers, 0, k - 1, missing_id);
			}
			end = clock();
			timeInterpolationSearchWorst += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;

			start = clock();
			for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
				interpolation_search(idSortedPlayers, 0, k - 1, averageInterpolationId);
			}
			end = clock();
			timeInterpolationSearchAverage += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;

			start = clock();
			for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
				binary_search_range(scoreRangePlayers, 0, k - 1, &scoreWorstTarget, compare_score, &first, &last);
			}
			end = clock();
			timeBinarySearchRangeWorst += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;

			first = -1;
			last = -1;
			start = clock();
			for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
				binary_search_range(scoreRangePlayers, 0, k - 1, &averageScoreTarget, compare_score, &first, &last);
			}
			end = clock();
			timeBinarySearchRangeAverage += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;
		}

		timeLinearSearchWorst /= NUM_TRIALS;
		timeLinearSearchAverage /= NUM_TRIALS;
		timeBinarySearchWorst /= NUM_TRIALS;
		timeBinarySearchAverage /= NUM_TRIALS;
		timeBinarySearchRecursiveWorst /= NUM_TRIALS;
		timeBinarySearchRecursiveAverage /= NUM_TRIALS;
		timeExponentialSearchWorst /= NUM_TRIALS;
		timeExponentialSearchAverage /= NUM_TRIALS;
		timeInterpolationSearchWorst /= NUM_TRIALS;
		timeInterpolationSearchAverage /= NUM_TRIALS;
		timeBinarySearchRangeWorst /= NUM_TRIALS;
		timeBinarySearchRangeAverage /= NUM_TRIALS;

		printf(PURPLE "║" MAG5 "\tLinear Worst:" WHITE " %.10f" PURPLE "         ║\n", timeLinearSearchWorst);
		printf(PURPLE "║" MAG5 "\tLinear Average:" WHITE " %.10f" PURPLE "       ║\n", timeLinearSearchAverage);
		printf(PURPLE "║" MAG6 "\tBinary Worst:" WHITE " %.10f" PURPLE "         ║\n", timeBinarySearchWorst);
		printf(PURPLE "║" MAG6 "\tBinary Average:" WHITE " %.10f" PURPLE "       ║\n", timeBinarySearchAverage);
		printf(PURPLE "║" MAG6 "\tBinary Rec Worst:" WHITE " %.10f" PURPLE "     ║\n", timeBinarySearchRecursiveWorst);
		printf(PURPLE "║" MAG6 "\tBinary Rec Average:" WHITE " %.10f" PURPLE "   ║\n", timeBinarySearchRecursiveAverage);
		printf(PURPLE "║" MAG6 "\tExponential Worst:" WHITE " %.10f" PURPLE "    ║\n", timeExponentialSearchWorst);
		printf(PURPLE "║" MAG6 "\tExponential Average:" WHITE " %.10f" PURPLE "  ║\n", timeExponentialSearchAverage);
		printf(PURPLE "║" MAG6 "\tInterpolation Worst:" WHITE " %.10f" PURPLE "  ║\n", timeInterpolationSearchWorst);
		printf(PURPLE "║" MAG6 "\tInterpolation Average:" WHITE " %.10f" PURPLE "║\n", timeInterpolationSearchAverage);
		printf(PURPLE "║" MAG6 "\tRange Score Worst:" WHITE " %.10f" PURPLE "    ║\n", timeBinarySearchRangeWorst);
		printf(PURPLE "║" MAG6 "\tRange Score Average:" WHITE " %.10f" PURPLE "  ║\n", timeBinarySearchRangeAverage);

		resultados[struct_idx].n = k;
		resultados[struct_idx].timeLinearSearchWorst = timeLinearSearchWorst;
		resultados[struct_idx].timeLinearSearchAverage = timeLinearSearchAverage;
		resultados[struct_idx].timeBinarySearchWorst = timeBinarySearchWorst;
		resultados[struct_idx].timeBinarySearchAverage = timeBinarySearchAverage;
		resultados[struct_idx].timeBinarySearchRecursiveWorst = timeBinarySearchRecursiveWorst;
		resultados[struct_idx].timeBinarySearchRecursiveAverage = timeBinarySearchRecursiveAverage;
		resultados[struct_idx].timeExponentialSearchWorst = timeExponentialSearchWorst;
		resultados[struct_idx].timeExponentialSearchAverage = timeExponentialSearchAverage;
		resultados[struct_idx].timeInterpolationSearchWorst = timeInterpolationSearchWorst;
		resultados[struct_idx].timeInterpolationSearchAverage = timeInterpolationSearchAverage;
		resultados[struct_idx].timeBinarySearchRangeWorst = timeBinarySearchRangeWorst;
		resultados[struct_idx].timeBinarySearchRangeAverage = timeBinarySearchRangeAverage;

		fprintf(csv,
				"%d,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f\n",
				resultados[struct_idx].n,
				resultados[struct_idx].timeLinearSearchWorst,
				resultados[struct_idx].timeLinearSearchAverage,
				resultados[struct_idx].timeBinarySearchWorst,
				resultados[struct_idx].timeBinarySearchAverage,
				resultados[struct_idx].timeBinarySearchRecursiveWorst,
				resultados[struct_idx].timeBinarySearchRecursiveAverage,
				resultados[struct_idx].timeExponentialSearchWorst,
				resultados[struct_idx].timeExponentialSearchAverage,
				resultados[struct_idx].timeInterpolationSearchWorst,
				resultados[struct_idx].timeInterpolationSearchAverage,
				resultados[struct_idx].timeBinarySearchRangeWorst,
				resultados[struct_idx].timeBinarySearchRangeAverage);
		printf(PURPLE "╠══════════════════════════════════════════╣\n");
		struct_idx++;
	}

	printf(PURPLE "║" LIGHT_GREEN "              CSV Data written" PURPLE "            ║\n");
	printf(PURPLE "╚══════════════════════════════════════════╝\n" RESET);
	fclose(csv);

	printf("\n" BG_GREEN "Data saved in build/db/experiment.csv" RESET "\n");

	free(players);
	free(linearSearchPlayers);
	free(idSortedPlayers);
	free(scoreRangePlayers);
}
