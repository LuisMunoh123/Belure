/**
 * @file generate_exec_times.c
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Funciones para la ejecution de tests
 */

#include "generate_exec_times.h"
#include "utilities.h"

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

static double measure_linear_search_time(Player players[], int n, Player *target)
{
	double total_time = 0.0;
	clock_t start, end;

	for (int i = 0; i < NUM_TRIALS; i++) {
		start = clock();
		for (int rep = 0; rep < LINEAR_REPEAT; rep++) {
			linear_search(players, n, target, compare_id);
		}
		end = clock();
		total_time += ((double)(end - start) / CLOCKS_PER_SEC) / LINEAR_REPEAT;
	}

	return total_time / NUM_TRIALS;
}

static double measure_linear_search_average_time(Player players[], int n)
{
	double total_time = 0.0;
	clock_t start, end;

	for (int i = 0; i < NUM_TRIALS; i++) {
		Player target = {players[rand() % n].id, "", "", 0.0f, 0};
		start = clock();
		for (int rep = 0; rep < LINEAR_REPEAT; rep++) {
			linear_search(players, n, &target, compare_id);
		}
		end = clock();
		total_time += ((double)(end - start) / CLOCKS_PER_SEC) / LINEAR_REPEAT;
	}

	return total_time / NUM_TRIALS;
}

static double measure_binary_search_time(Player players[], int n, Player *target)
{
	double total_time = 0.0;
	clock_t start, end;

	for (int i = 0; i < NUM_TRIALS; i++) {
		start = clock();
		for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
			binary_search_iterative(players, 0, n - 1, target, compare_id);
		}
		end = clock();
		total_time += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;
	}

	return total_time / NUM_TRIALS;
}

static double measure_binary_search_average_time(Player players[], int n)
{
	double total_time = 0.0;
	clock_t start, end;

	for (int i = 0; i < NUM_TRIALS; i++) {
		Player target = {players[rand() % n].id, "", "", 0.0f, 0};
		start = clock();
		for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
			binary_search_iterative(players, 0, n - 1, &target, compare_id);
		}
		end = clock();
		total_time += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;
	}

	return total_time / NUM_TRIALS;
}

static double measure_binary_search_recursive_time(Player players[], int n, Player *target)
{
	double total_time = 0.0;
	clock_t start, end;

	for (int i = 0; i < NUM_TRIALS; i++) {
		start = clock();
		for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
			binary_search_recursive(players, 0, n - 1, target, compare_id);
		}
		end = clock();
		total_time += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;
	}

	return total_time / NUM_TRIALS;
}

static double measure_binary_search_recursive_average_time(Player players[], int n)
{
	double total_time = 0.0;
	clock_t start, end;

	for (int i = 0; i < NUM_TRIALS; i++) {
		Player target = {players[rand() % n].id, "", "", 0.0f, 0};
		start = clock();
		for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
			binary_search_recursive(players, 0, n - 1, &target, compare_id);
		}
		end = clock();
		total_time += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;
	}

	return total_time / NUM_TRIALS;
}

static double measure_exponential_search_time(Player players[], int n, Player *target)
{
	double total_time = 0.0;
	clock_t start, end;

	for (int i = 0; i < NUM_TRIALS; i++) {
		start = clock();
		for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
			exponential_search(players, n, target, compare_id);
		}
		end = clock();
		total_time += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;
	}

	return total_time / NUM_TRIALS;
}

static double measure_exponential_search_average_time(Player players[], int n)
{
	double total_time = 0.0;
	clock_t start, end;

	for (int i = 0; i < NUM_TRIALS; i++) {
		Player target = {players[rand() % n].id, "", "", 0.0f, 0};
		start = clock();
		for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
			exponential_search(players, n, &target, compare_id);
		}
		end = clock();
		total_time += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;
	}

	return total_time / NUM_TRIALS;
}

static double measure_interpolation_search_time(Player players[], int n, int target_id)
{
	double total_time = 0.0;
	clock_t start, end;

	for (int i = 0; i < NUM_TRIALS; i++) {
		start = clock();
		for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
			interpolation_search(players, 0, n - 1, target_id);
		}
		end = clock();
		total_time += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;
	}

	return total_time / NUM_TRIALS;
}

static double measure_interpolation_search_average_time(Player players[], int n)
{
	double total_time = 0.0;
	clock_t start, end;

	for (int i = 0; i < NUM_TRIALS; i++) {
		int target_id = players[rand() % n].id;
		start = clock();
		for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
			interpolation_search(players, 0, n - 1, target_id);
		}
		end = clock();
		total_time += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;
	}

	return total_time / NUM_TRIALS;
}

static double measure_binary_search_range_time(Player players[], int n, Player *target)
{
	double total_time = 0.0;
	clock_t start, end;

	for (int i = 0; i < NUM_TRIALS; i++) {
		int first = -1;
		int last = -1;
		start = clock();
		for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
			binary_search_range(players, 0, n - 1, target, compare_score, &first, &last);
		}
		end = clock();
		total_time += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;
	}

	return total_time / NUM_TRIALS;
}

static double measure_binary_search_range_average_time(Player players[], int n)
{
	double total_time = 0.0;
	clock_t start, end;

	for (int i = 0; i < NUM_TRIALS; i++) {
		Player target = {0, "", "", players[rand() % n].score, 0};
		int first = -1;
		int last = -1;
		start = clock();
		for (int rep = 0; rep < SEARCH_REPEAT; rep++) {
			binary_search_range(players, 0, n - 1, &target, compare_score, &first, &last);
		}
		end = clock();
		total_time += ((double)(end - start) / CLOCKS_PER_SEC) / SEARCH_REPEAT;
	}

	return total_time / NUM_TRIALS;
}

void run_experiment()
{
	int option = 0;

	printf(PURPLE "\n╔════════════════════════════════════╗\n");
	printf(PURPLE "║" WHITE "        MENU DE EXPERIMENTOS       " PURPLE " ║\n");
	printf(PURPLE "╠════════════════════════════════════╣\n");
	printf(PURPLE "║" LIGHT_BLUE " 1. Ordenamiento                    " PURPLE "║\n");
	printf(PURPLE "║" LIGHT_GREEN " 2. Busqueda                        " PURPLE "║\n");
	printf(PURPLE "╚════════════════════════════════════╝\n" RESET);
	printf("Selecciona una opcion: ");

	if (scanf("%d", &option) != 1) {
		printf("Error: opcion invalida.\n");
		return;
	}

	if (option == 1) {
		run_sort_experiment();
	} else if (option == 2) {
		run_search_experiment();
	} else {
		printf("Error: opcion invalida.\n");
	}
}

void run_sort_experiment()
{
	int n = 0;
	Player* players;
	if ((players = load_players("build/db/players.csv", &n)) == NULL) {
		return;
	}

	printf("Arreglo de jugadores cargado\n");
	print_player_array(players, n);

	const int num_points = NUM_STEPS;
	SortExecResults resultados[num_points];
	int struct_idx = 0;

	Player* swapSortPlayers = malloc(n * sizeof(Player));
	Player* insertionSortPlayers = malloc(n * sizeof(Player));
	Player* selectionSortPlayers = malloc(n * sizeof(Player));
	Player* cocktailShakerSortPlayers = malloc(n * sizeof(Player));
	Player* quickSortPlayers = malloc(n * sizeof(Player));
	Player* mergeSortPlayers = malloc(n * sizeof(Player));
	Player* mergeSortOptimizedPlayers = malloc(n * sizeof(Player));

	if (!swapSortPlayers || !insertionSortPlayers || !selectionSortPlayers || !cocktailShakerSortPlayers || !quickSortPlayers || !mergeSortPlayers || !mergeSortOptimizedPlayers) {
		free(swapSortPlayers);
		free(insertionSortPlayers);
		free(selectionSortPlayers);
		free(cocktailShakerSortPlayers);
		free(quickSortPlayers);
		free(mergeSortPlayers);
		free(mergeSortOptimizedPlayers);
		free(players);
		print_error(102, "players", NULL);
		return;
	}

	FILE *csv = fopen("build/db/experiment.csv", "w");
	if (csv == NULL) {
		free(players);
		free(swapSortPlayers);
		free(insertionSortPlayers);
		free(selectionSortPlayers);
		free(cocktailShakerSortPlayers);
		free(quickSortPlayers);
		free(mergeSortPlayers);
		free(mergeSortOptimizedPlayers);
		print_error(101, "build/db/experiment.csv", NULL);
		return;
	}

	fprintf(csv, "N,Bubble Sort,Insertion Sort,Selection Sort,Cocktail Shaker Sort,Quick Sort,Merge Sort,Merge Sort Optimized\n");

	printf(PURPLE "╔══════════════════════════════════════════╗\n");

	for (int s = 0; s < num_points; s++) {
		int k = (num_points == 1) ? n : 1 + (s * (n - 1)) / (num_points - 1);
		clock_t start, end;

		printf(PURPLE "║" MAGENTA "Procesando n =" WHITE" %8d" PURPLE "                   ║\n", k);
		printf(PURPLE "║" MAGENTA "Paso:" WHITE" %4d/%4d" PURPLE "                           ║\n", s + 1, num_points);

		double timeSwapSort = 0;
		for (int i = 0; i < NUM_TRIALS; i++) {
			memcpy(swapSortPlayers, players, k * sizeof(Player));
			start = clock();
			swap_sort(swapSortPlayers, k, compare_id);
			end = clock();
			timeSwapSort += (double)(end - start) / CLOCKS_PER_SEC;
		}
		timeSwapSort /= NUM_TRIALS;
		printf(PURPLE "║" MAG1 "\tSwap Sort:" WHITE " %f\t" PURPLE "           ║\n", timeSwapSort);

		double timeInsertionSort = 0;
		for (int i = 0; i < NUM_TRIALS; i++) {
			memcpy(insertionSortPlayers, players, k * sizeof(Player));
			start = clock();
			insertion_sort(insertionSortPlayers, k, compare_id);
			end = clock();
			timeInsertionSort += (double)(end - start) / CLOCKS_PER_SEC;
		}
		timeInsertionSort /= NUM_TRIALS;
		printf(PURPLE "║" MAG2 "\tInsertion Sort:" WHITE " %f\t" PURPLE "   ║\n", timeInsertionSort);

		double timeSelectionSort = 0;
		for (int i = 0; i < NUM_TRIALS; i++) {
			memcpy(selectionSortPlayers, players, k * sizeof(Player));
			start = clock();
			selection_sort(selectionSortPlayers, k, compare_id);
			end = clock();
			timeSelectionSort += (double)(end - start) / CLOCKS_PER_SEC;
		}
		timeSelectionSort /= NUM_TRIALS;
		printf(PURPLE "║" MAG3 "\tSelection Sort:" WHITE " %f\t" PURPLE "   ║\n", timeSelectionSort);

		double timeCocktailSort = 0;
		for (int i = 0; i < NUM_TRIALS; i++) {
			memcpy(cocktailShakerSortPlayers, players, k * sizeof(Player));
			start = clock();
			cocktail_shaker_sort(cocktailShakerSortPlayers, k, compare_id);
			end = clock();
			timeCocktailSort += (double)(end - start) / CLOCKS_PER_SEC;
		}
		timeCocktailSort /= NUM_TRIALS;
		printf(PURPLE "║" MAG4 "\tCocktail Shaker Sort:" WHITE " %f" PURPLE "     ║\n", timeCocktailSort);

		double timeQuickSort = 0;
		for (int i = 0; i < NUM_TRIALS; i++) {
			memcpy(quickSortPlayers, players, k * sizeof(Player));
			start = clock();
			quick_sort(quickSortPlayers, 0, k - 1, compare_id);
			end = clock();
			timeQuickSort += (double)(end - start) / CLOCKS_PER_SEC;
		}
		timeQuickSort /= NUM_TRIALS;
		printf(PURPLE "║" MAG5 "\tQuick Sort:" WHITE "    %f" PURPLE "            ║\n", timeQuickSort);

		double timeMergeSort = 0;
		for (int i = 0; i < NUM_TRIALS; i++) {
			memcpy(mergeSortPlayers, players, k * sizeof(Player));
			start = clock();
			merge_sort_classic(mergeSortPlayers, 0, k - 1, compare_id);
			end = clock();
			timeMergeSort += (double)(end - start) / CLOCKS_PER_SEC;
		}
		timeMergeSort /= NUM_TRIALS;
		printf(PURPLE "║" MAG5 "\tMerge Sort:" WHITE "    %f" PURPLE "            ║\n", timeMergeSort);

		double timeMergeOptimized = 0;
		for (int i = 0; i < NUM_TRIALS; i++) {
			memcpy(mergeSortOptimizedPlayers, players, k * sizeof(Player));
			start = clock();
			merge_sort_optimized(mergeSortOptimizedPlayers, 0, k - 1, MERGE_THRESHOLD, compare_id);
			end = clock();
			timeMergeOptimized += (double)(end - start) / CLOCKS_PER_SEC;
		}
		timeMergeOptimized /= NUM_TRIALS;
		printf(PURPLE "║" MAG5 "\tMerge Optimized:" WHITE " %f" PURPLE "          ║\n", timeMergeOptimized);

		resultados[struct_idx].n = k;
		resultados[struct_idx].timeSwapSort = timeSwapSort;
		resultados[struct_idx].timeInsertionSort = timeInsertionSort;
		resultados[struct_idx].timeSelectionSort = timeSelectionSort;
		resultados[struct_idx].timeCocktailSort = timeCocktailSort;
		resultados[struct_idx].timeQuickSort = timeQuickSort;
		resultados[struct_idx].timeMergeSort = timeMergeSort;
		resultados[struct_idx].timeMergeSortOptimized = timeMergeOptimized;

		fprintf(csv,
				"%d,%f,%f,%f,%f,%f,%f,%f\n",
				resultados[struct_idx].n,
				resultados[struct_idx].timeSwapSort,
				resultados[struct_idx].timeInsertionSort,
				resultados[struct_idx].timeSelectionSort,
				resultados[struct_idx].timeCocktailSort,
				resultados[struct_idx].timeQuickSort,
				resultados[struct_idx].timeMergeSort,
				resultados[struct_idx].timeMergeSortOptimized);
		printf(PURPLE "╠══════════════════════════════════════════╣\n");
		struct_idx++;
	}

	printf(PURPLE "║" LIGHT_GREEN "              CSV Data written" PURPLE "            ║\n");
	printf(PURPLE "╚══════════════════════════════════════════╝\n" RESET);
	fclose(csv);

	printf("\n" BG_GREEN "Data saved in build/db/experiment.csv" RESET "\n");

	free(players);
	free(swapSortPlayers);
	free(insertionSortPlayers);
	free(selectionSortPlayers);
	free(cocktailShakerSortPlayers);
	free(quickSortPlayers);
	free(mergeSortPlayers);
	free(mergeSortOptimizedPlayers);
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
		Player linearWorstTarget;
		Player binaryWorstTarget;
		Player scoreWorstTarget;
		int interpolationWorstTargetId;

		printf(PURPLE "║" MAGENTA "Procesando n =" WHITE" %8d" PURPLE "                   ║\n", k);
		printf(PURPLE "║" MAGENTA "Paso:" WHITE" %4d/%4d" PURPLE "                           ║\n", s + 1, num_points);

		linearWorstTarget.id = idSortedPlayers[k - 1].id + 1;
		binaryWorstTarget.id = pick_missing_id(idSortedPlayers, k);
		interpolationWorstTargetId = binaryWorstTarget.id;
		scoreWorstTarget.score = pick_missing_score(scoreRangePlayers, k);

		double timeLinearSearchWorst = measure_linear_search_time(linearSearchPlayers, k, &linearWorstTarget);
		double timeLinearSearchAverage = measure_linear_search_average_time(linearSearchPlayers, k);
		double timeBinarySearchWorst = measure_binary_search_time(idSortedPlayers, k, &binaryWorstTarget);
		double timeBinarySearchAverage = measure_binary_search_average_time(idSortedPlayers, k);
		double timeBinarySearchRecursiveWorst = measure_binary_search_recursive_time(idSortedPlayers, k, &binaryWorstTarget);
		double timeBinarySearchRecursiveAverage = measure_binary_search_recursive_average_time(idSortedPlayers, k);
		double timeExponentialSearchWorst = measure_exponential_search_time(idSortedPlayers, k, &binaryWorstTarget);
		double timeExponentialSearchAverage = measure_exponential_search_average_time(idSortedPlayers, k);
		double timeInterpolationSearchWorst = measure_interpolation_search_time(idSortedPlayers, k, interpolationWorstTargetId);
		double timeInterpolationSearchAverage = measure_interpolation_search_average_time(idSortedPlayers, k);
		double timeBinarySearchRangeWorst = measure_binary_search_range_time(scoreRangePlayers, k, &scoreWorstTarget);
		double timeBinarySearchRangeAverage = measure_binary_search_range_average_time(scoreRangePlayers, k);

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
