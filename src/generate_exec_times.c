/**
 * @file generate_exec_times.c
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Functions for running tests.
 */

#include "generate_exec_times.h"
#include "utilities.h"

/**
 * @brief Runs a sorting and search experiment, generating a CSV with the results.
 * 
 */
void run_experiment()
{
	int n = 0; // Cantidad de jugadores
	Player* players;
	if ((players = load_players("build/db/players.csv", &n)) == NULL) {
			return;
	}
	printf("Player array loaded\n");
	print_player_array(players, n);

	const int num_points = NUM_STEPS;
	ExecResults resultados[num_points];
	int struct_idx = 0;

	// Inicializamos los arreglos de jugadores
	Player* swapSortPlayers = malloc(n * sizeof(Player));
	Player* insertionSortPlayers = malloc(n * sizeof(Player));
	Player* selectionSortPlayers = malloc(n * sizeof(Player));
	Player* cocktailShakerSortPlayers = malloc(n * sizeof(Player));
	Player* quickSortPlayers = malloc(n * sizeof(Player));
	Player* mergeSortPlayers = malloc(n * sizeof(Player));
	Player* mergeSortOptimizedPlayers = malloc(n * sizeof(Player));
	Player* linearSearchPlayers = malloc(n * sizeof(Player));
	Player* binarySearchPlayers = malloc(n * sizeof(Player));

	if (!swapSortPlayers || !insertionSortPlayers || !selectionSortPlayers || !cocktailShakerSortPlayers || !quickSortPlayers || !mergeSortPlayers || !mergeSortOptimizedPlayers || !linearSearchPlayers || !binarySearchPlayers) {
		free(swapSortPlayers);
		free(insertionSortPlayers);
		free(selectionSortPlayers);
		free(cocktailShakerSortPlayers);
		free(quickSortPlayers);
		free(mergeSortPlayers);
		free(mergeSortOptimizedPlayers);
		free(linearSearchPlayers);
		free(binarySearchPlayers);
		print_error(102, "players", NULL);
		return;
	}

	// Abrimos el archivo de resultados
	FILE *csv = fopen("build/db/experiment.csv", "w");
	if (csv == NULL)
	{
		print_error(101, "build/db/experiment.csv", NULL);
		return;
	}

	fprintf(csv, "N,Bubble Sort,Insertion Sort,Selection Sort,Cocktail Shaker Sort,Quick Sort,Merge Sort,Merge Sort Optimized,Linear Search,Binary Search\n");

	// header
	printf(PURPLE "╔══════════════════════════════════════════╗\n");

	for (int s = 0; s < num_points; s++) {
		// k grows from 1 to n, distributed across NUM_STEPS points.
		// The formula avoids k = 0 and guarantees that the last point is n.
		int k = (num_points == 1) ? n : 1 + (s * (n - 1)) / (num_points - 1);

		Player target = {n+1, "", "", 0.0, 0}; // Example target player

		printf(PURPLE "║" MAGENTA "Processing n =" WHITE" %8d" PURPLE "                   ║\n", k);
		printf(PURPLE "║" MAGENTA "Step:" WHITE" %4d/%4d" PURPLE "                           ║\n", s+1, num_points);
		//printf(PURPLE "║                                          ║\n");
		clock_t start, end;

		// Bubble Sort timings
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
 

		// Insertion Sort timings
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
		
		// Selection Sort timings
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

		// Cocktail Shaker Sort timings
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

		// Quick Sort timings
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


		// Merge Sort timings
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

		// Optimized Merge Sort timings
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

		// Linear Search timings
		double timeLinearSearch = 0;
		for (int i = 0; i < NUM_TRIALS; i++) {
			memcpy(linearSearchPlayers, players, k * sizeof(Player));
			start = clock();
			linear_search(linearSearchPlayers, k, &target, compare_id);
			end = clock();
			timeLinearSearch += (double)(end - start) / CLOCKS_PER_SEC;
		}
		timeLinearSearch /= NUM_TRIALS;
		printf( PURPLE "║" MAG5 "\tLinear Search:" WHITE " %f" PURPLE "            ║\n", timeLinearSearch);

		// Binary Search timings
		// We use an already sorted array as input
		double timeBinarySearch = 0;
		for (int i = 0; i < NUM_TRIALS; i++) {
			memcpy(binarySearchPlayers, cocktailShakerSortPlayers, k * sizeof(Player));
			start = clock();
			binary_search_iterative(binarySearchPlayers, 0, k - 1, &target, compare_id);
			end = clock();
			timeBinarySearch += (double)(end - start) / CLOCKS_PER_SEC;
		}
		timeBinarySearch /= NUM_TRIALS;
		printf( PURPLE "║" MAG6 "\tBinary Search:" WHITE " %f" PURPLE "            ║\n", timeBinarySearch);

		resultados[struct_idx].n = k;
		resultados[struct_idx].timeSwapSort = timeSwapSort;
		resultados[struct_idx].timeInsertionSort = timeInsertionSort;
		resultados[struct_idx].timeSelectionSort = timeSelectionSort;
		resultados[struct_idx].timeCocktailSort = timeCocktailSort;
		resultados[struct_idx].timeQuickSort = timeQuickSort;
		resultados[struct_idx].timeMergeSort = timeMergeSort;
		resultados[struct_idx].timeMergeSortOptimized = timeMergeOptimized;
		resultados[struct_idx].timeLinearSearch = timeLinearSearch;
		resultados[struct_idx].timeBinarySearch = timeBinarySearch;

		// Save to CSV
		fprintf(csv,
				"%d,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
				resultados[struct_idx].n,
				resultados[struct_idx].timeSwapSort,
				resultados[struct_idx].timeInsertionSort,
				resultados[struct_idx].timeSelectionSort,
				resultados[struct_idx].timeCocktailSort,
				resultados[struct_idx].timeQuickSort,
				resultados[struct_idx].timeMergeSort,
				resultados[struct_idx].timeMergeSortOptimized,
				resultados[struct_idx].timeLinearSearch,
				resultados[struct_idx].timeBinarySearch
			);
		printf(PURPLE "╠══════════════════════════════════════════╣\n");
		struct_idx++;
	}
	// End of table
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
	free(linearSearchPlayers);
	free(binarySearchPlayers);
}