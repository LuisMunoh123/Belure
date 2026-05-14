/**
 * @file sort_experiment.c
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Functions for running sorting experiments.
 */

#include "sort_experiment.h"
#include "utilities.h"

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
