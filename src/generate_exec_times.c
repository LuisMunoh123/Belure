/**
 * @file generate_exec_times.c
 * @author Andres Barbosa, Milton Hernandez, Ivan Gallardo
 * @brief Funciones para la ejecution de tests
 */

#include "generate_exec_times.h"
#include "utilities.h"

/**
 * @brief Ejecuta un experimento de ordenamiento y busqueda generando un CSV con los resultados
 * 
 */
void run_experiment()
{
	int n = 0; // Cantidad de jugadores
	Player* players;
	if ((players = load_players("build/db/players.csv", &n)) == NULL) {
			return;
	}
	printf("Arreglo de jugadores cargado\n");
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
	Player* mergeClassicPlayers = malloc(n * sizeof(Player));
	Player* mergeOpt4Players = malloc(n * sizeof(Player));
	Player* mergeOpt8Players = malloc(n * sizeof(Player));
	Player* mergeOpt16Players = malloc(n * sizeof(Player));
	Player* mergeOpt32Players = malloc(n * sizeof(Player));
	Player* linearSearchPlayers = malloc(n * sizeof(Player));
	Player* binarySearchPlayers = malloc(n * sizeof(Player));

	if (!swapSortPlayers || !insertionSortPlayers || !selectionSortPlayers || !cocktailShakerSortPlayers || !quickSortPlayers || !mergeClassicPlayers || !mergeOpt4Players || !mergeOpt8Players ||
    !mergeOpt16Players || !mergeOpt32Players || !linearSearchPlayers || !binarySearchPlayers) {
		free(swapSortPlayers);
		free(insertionSortPlayers);
		free(selectionSortPlayers);
		free(cocktailShakerSortPlayers);
		free(quickSortPlayers);
		free(linearSearchPlayers);
		free(binarySearchPlayers);
		free(mergeClassicPlayers);
		free(mergeOpt4Players);
		free(mergeOpt8Players);
		free(mergeOpt16Players);
		free(mergeOpt32Players);
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

	fprintf(csv, "N,Bubble Sort,Insertion Sort,Selection Sort,Cocktail Shaker Sort,Quick Sort,LMerge Classic,Merge Opt 4,Merge Opt 8,Merge Opt 16,Merge Opt 32,Linear Search,Binary Search\n");

	// header
	printf(PURPLE "╔══════════════════════════════════════════╗\n");

	for (int s = 0; s < num_points; s++) {
		// k crece desde 1 hasta n, repartido en NUM_STEPS puntos.
		// La fórmula evita k = 0 y garantiza que el último punto sea n.
		int k = (num_points == 1) ? n : 1 + (s * (n - 1)) / (num_points - 1);

		Player target = {n+1, "", "", 0.0, 0}; // Jugador buscado de ejemplo

		printf(PURPLE "║" MAGENTA "Procesando n =" WHITE" %8d" PURPLE "                   ║\n", k);
		printf(PURPLE "║" MAGENTA "Paso:" WHITE" %4d/%4d" PURPLE "                           ║\n", s+1, num_points);
		//printf(PURPLE "║                                          ║\n");
		clock_t start, end;

		// Tiempos Bubble Sort
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
 

		// Tiempos Insertion Sort
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
		
		// Tiempos Selection Sort
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

		// Tiempos Cocktail Shaker Sort
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

		// Tiempos Quick Sort
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

		// tiempo de merge sort clasico
		double timeMergeClassic = 0;
		for (int i = 0; i < NUM_TRIALS; i++) {
    			memcpy(mergeClassicPlayers, players, k * sizeof(Player));
    			start = clock();
    			merge_sort_classic(mergeClassicPlayers, 0, k - 1, compare_id);
    			end = clock();
    			timeMergeClassic += (double)(end - start) / CLOCKS_PER_SEC;
		}
		timeMergeClassic /= NUM_TRIALS;
		printf(PURPLE "║" MAG6 "\tMerge Classic:" WHITE " %f" PURPLE "          ║\n", timeMergeClassic);

		// tiempo de merge sort optimizado con threshold 4
		double timeMergeOpt4 = 0;
		for (int i = 0; i < NUM_TRIALS; i++) {
    			memcpy(mergeOpt4Players, players, k * sizeof(Player));
    			start = clock();
    			merge_sort_optimized(mergeOpt4Players, 0, k - 1, 4, compare_id);
    			end = clock();
    			timeMergeOpt4 += (double)(end - start) / CLOCKS_PER_SEC;
		}
		timeMergeOpt4 /= NUM_TRIALS;
		printf(PURPLE "║" MAG1 "\tMerge Opt 4:" WHITE "   %f" PURPLE "          ║\n", timeMergeOpt4);

		// tiempo de merge sort optimizado con threshold 8
		double timeMergeOpt8 = 0;
		for (int i = 0; i < NUM_TRIALS; i++) {
    			memcpy(mergeOpt8Players, players, k * sizeof(Player));
    			start = clock();
    			merge_sort_optimized(mergeOpt8Players, 0, k - 1, 8, compare_id);
    			end = clock();
    			timeMergeOpt8 += (double)(end - start) / CLOCKS_PER_SEC;
		}
			timeMergeOpt8 /= NUM_TRIALS;
			printf(PURPLE "║" MAG2 "\tMerge Opt 8:" WHITE "   %f" PURPLE "          ║\n", timeMergeOpt8);

		// tiempo de merge sort optimizado con threshold 16
		double timeMergeOpt16 = 0;
		for (int i = 0; i < NUM_TRIALS; i++) {
    			memcpy(mergeOpt16Players, players, k * sizeof(Player));
    			start = clock();
    			merge_sort_optimized(mergeOpt16Players, 0, k - 1, 16, compare_id);
    			end = clock();
    			timeMergeOpt16 += (double)(end - start) / CLOCKS_PER_SEC;
		}
			timeMergeOpt16 /= NUM_TRIALS;
			printf(PURPLE "║" MAG3 "\tMerge Opt 16:" WHITE "  %f" PURPLE "          ║\n", timeMergeOpt16);

		// tiempo de merge sort optimizado con threshold 32
		double timeMergeOpt32 = 0;
		for (int i = 0; i < NUM_TRIALS; i++) {
    			memcpy(mergeOpt32Players, players, k * sizeof(Player));
    			start = clock();
    			merge_sort_optimized(mergeOpt32Players, 0, k - 1, 32, compare_id);
    			end = clock();
    			timeMergeOpt32 += (double)(end - start) / CLOCKS_PER_SEC;
		}
			timeMergeOpt32 /= NUM_TRIALS;
			printf(PURPLE "║" MAG4 "\tMerge Opt 32:" WHITE "  %f" PURPLE "          ║\n", timeMergeOpt32);

		// Tiempos Linear Search
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

		// Tiempos Binary Search
		// USAMOS UN ARREGLO DE LOS YA ORDENADOS COMO INPUT
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
		resultados[struct_idx].timeMergeClassic = timeMergeClassic;
		resultados[struct_idx].timeMergeOpt4 = timeMergeOpt4;
		resultados[struct_idx].timeMergeOpt8 = timeMergeOpt8;
		resultados[struct_idx].timeMergeOpt16 = timeMergeOpt16;
		resultados[struct_idx].timeMergeOpt32 = timeMergeOpt32;
		resultados[struct_idx].timeLinearSearch = timeLinearSearch;
		resultados[struct_idx].timeBinarySearch = timeBinarySearch;

		// Guardar en CSV
		fprintf(csv,
				"%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
				resultados[struct_idx].n,
				resultados[struct_idx].timeSwapSort,
				resultados[struct_idx].timeInsertionSort,
				resultados[struct_idx].timeSelectionSort,
				resultados[struct_idx].timeCocktailSort,
				resultados[struct_idx].timeQuickSort,
				resultados[struct_idx].timeMergeClassic,
        			resultados[struct_idx].timeMergeOpt4,
        			resultados[struct_idx].timeMergeOpt8,
        			resultados[struct_idx].timeMergeOpt16,
        			resultados[struct_idx].timeMergeOpt32,
				resultados[struct_idx].timeLinearSearch,
				resultados[struct_idx].timeBinarySearch
			);
		printf(PURPLE "╠══════════════════════════════════════════╣\n");
		struct_idx++;
	}
	// end of table
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
	free(mergeClassicPlayers);
	free(mergeOpt4Players);
 	free(mergeOpt8Players);
	free(mergeOpt16Players);
	free(mergeOpt32Players);
	free(linearSearchPlayers);
	free(binarySearchPlayers);
}
