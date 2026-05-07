/**
 * @file generate_exec_times.c
 * @author Andres Barbosa, Milton Hernandez, Ivan Gallardo
 * @brief Funciones para la ejecution de tests
 */

#include "generate_exec_times.h"
#include "utilities.h"

/**
 * @brief compara insertion sort vs merge sort clasico para observar
 *        desde que tamano merge sort empieza a ser mas eficiente.
 *        el punto de cruce es el threshold optimo de optimizacion.
 */
void run_experiment()
{
	int n = 0; // cantidad de jugadores cargados
	Player* players;
	if ((players = load_players("build/db/players.csv", &n)) == NULL) {
		return;
	}
	printf("Arreglo de jugadores cargado\n");
	print_player_array(players, n);

	const int num_points = NUM_STEPS;
	ExecResults resultados[num_points];
	int struct_idx = 0;

	// arreglos de trabajo para cada algoritmo
	Player* insertionSortPlayers = malloc(n * sizeof(Player));
	Player* mergeClassicPlayers  = malloc(n * sizeof(Player));

	if (!insertionSortPlayers || !mergeClassicPlayers) {
		free(insertionSortPlayers);
		free(mergeClassicPlayers);
		print_error(102, "players", NULL);
		return;
	}

	// abrimos el archivo de resultados
	FILE *csv = fopen("build/db/experiment.csv", "w");
	if (csv == NULL) {
		free(insertionSortPlayers);
		free(mergeClassicPlayers);
		free(players);
		print_error(101, "build/db/experiment.csv", NULL);
		return;
	}

	// cabecera del csv: nombres con "sort" para que el graficador los detecte
	fprintf(csv, "N,Insertion Sort,Merge Sort Classic\n");

	// encabezado de la tabla en consola
	printf(PURPLE "╔══════════════════════════════════════════╗\n");

	for (int s = 0; s < num_points; s++) {
		// k crece desde 1 hasta n, repartido en NUM_STEPS puntos.
		// la formula evita k = 0 y garantiza que el ultimo punto sea n.
		int k = (num_points == 1) ? n : 1 + (s * (n - 1)) / (num_points - 1);

		printf(PURPLE "║" MAGENTA "Procesando n =" WHITE " %8d" PURPLE "                   ║\n", k);
		printf(PURPLE "║" MAGENTA "Paso:" WHITE " %4d/%4d" PURPLE "                           ║\n", s + 1, num_points);

		clock_t start, end;

		// tiempos insertion sort
		double timeInsertionSort = 0;
		for (int i = 0; i < NUM_TRIALS; i++) {
			memcpy(insertionSortPlayers, players, k * sizeof(Player));
			start = clock();
			insertion_sort(insertionSortPlayers, k, compare_id);
			end = clock();
			timeInsertionSort += (double)(end - start) / CLOCKS_PER_SEC;
		}
		timeInsertionSort /= NUM_TRIALS;
		printf(PURPLE "║" MAG2 "\tInsertion Sort:    " WHITE " %f" PURPLE "     ║\n", timeInsertionSort);

		// tiempos merge sort clasico
		double timeMergeClassic = 0;
		for (int i = 0; i < NUM_TRIALS; i++) {
			memcpy(mergeClassicPlayers, players, k * sizeof(Player));
			start = clock();
			merge_sort_classic(mergeClassicPlayers, 0, k - 1, compare_id);
			end = clock();
			timeMergeClassic += (double)(end - start) / CLOCKS_PER_SEC;
		}
		timeMergeClassic /= NUM_TRIALS;
		printf(PURPLE "║" MAG6 "\tMerge Sort Classic:" WHITE " %f" PURPLE "     ║\n", timeMergeClassic);

		// guardar resultados en el struct
		resultados[struct_idx].n               = k;
		resultados[struct_idx].timeInsertionSort = timeInsertionSort;
		resultados[struct_idx].timeMergeClassic  = timeMergeClassic;

		// escribir fila en el csv
		fprintf(csv, "%d,%f,%f\n",
			resultados[struct_idx].n,
			resultados[struct_idx].timeInsertionSort,
			resultados[struct_idx].timeMergeClassic
		);

		printf(PURPLE "╠══════════════════════════════════════════╣\n");
		struct_idx++;
	}

	// cierre de la tabla
	printf(PURPLE "║" LIGHT_GREEN "              CSV Data written" PURPLE "            ║\n");
	printf(PURPLE "╚══════════════════════════════════════════╝\n" RESET);
	fclose(csv);

	printf("\n" BG_GREEN "Data saved in build/db/experiment.csv" RESET "\n");

	free(players);
	free(insertionSortPlayers);
	free(mergeClassicPlayers);
}
