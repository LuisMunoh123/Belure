/**
 * @file generate_exec_times.c
 * @author Andres Barbosa, Milton Hernandez, Ivan Gallardo
 * @brief Funciones para la ejecution de tests
 */

#include "generate_exec_times.h"

/**
 * @brief Ejecuta un experimento de ordenamiento y busqueda generando un CSV con los resultados
 * 
 */
void run_experiment(void)
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
    Player* linearSearchPlayers = malloc(n * sizeof(Player));
    Player* binarySearchPlayers = malloc(n * sizeof(Player));

    if (!swapSortPlayers || !insertionSortPlayers || !selectionSortPlayers || !cocktailShakerSortPlayers || !linearSearchPlayers || !binarySearchPlayers) {
        free(swapSortPlayers);
        free(insertionSortPlayers);
        free(selectionSortPlayers);
        free(cocktailShakerSortPlayers);
        free(linearSearchPlayers);
        free(binarySearchPlayers);
        print_error(102, "players", NULL);
        return;
    }

    // Abrimos el archivo de resultados
    FILE *csv = fopen("build/db/comparar_Algoritmos.csv", "w");
    if (csv == NULL)
    {
        print_error(101, "build/db/comparar_Algoritmos.csv", NULL);
        return;
    }

    fprintf(csv, "N,Bubble Sort,Insertion Sort,Selection Sort,Cocktail Shaker Sort,Linear Search,Binary Search\n");


    for (int s = 0; s < num_points; s++) {
        // k crece desde 1 hasta n, repartido en NUM_STEPS puntos.
        // La fórmula evita k = 0 y garantiza que el último punto sea n.
        int k = (num_points == 1) ? n : 1 + (s * (n - 1)) / (num_points - 1);

        Player target = {k-1, "", "", 0.0, 0, false}; // Jugador buscado de ejemplo
        printf("Procesando n = %d\n", k);
        clock_t start, end;

        // Tiempos Bubble Sort
        double timeSwapSort = 0;
        for (int i = 0; i < NUM_TRIALS; i++) {
            memcpy(swapSortPlayers, players, k * sizeof(Player)); //TODO: Comprobar que se hizo bien
            start = clock();
            swap_sort(swapSortPlayers, k, compare_id);
            end = clock();
            timeSwapSort += (double)(end - start) / CLOCKS_PER_SEC;
        }
        timeSwapSort /= NUM_TRIALS;
        printf("\tSwap Sort: %f\n", timeSwapSort);


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
        printf("\tInsertion Sort: %f\n", timeInsertionSort);
        
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
        printf("\tSelection Sort: %f\n", timeSelectionSort);

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
        printf("\tCocktail Shaker Sort: %f\n", timeCocktailSort);

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
        printf("\tLinear Search: %f\n", timeLinearSearch);

        // Tiempos Binary Search
        // USAMOS UN ARREGLO DE LOS YA ORDENADOS COMO INPUT
        double timeBinarySearch = 0;
        for (int i = 0; i < NUM_TRIALS; i++) {
            memcpy(binarySearchPlayers, cocktailShakerSortPlayers, k * sizeof(Player));
            start = clock();
            binary_search(binarySearchPlayers, 0, k - 1, &target);
            end = clock();
            timeBinarySearch += (double)(end - start) / CLOCKS_PER_SEC;
        }
        timeBinarySearch /= NUM_TRIALS;
        printf("\tBinary Search: %f\n", timeBinarySearch);

        resultados[struct_idx].n = k;
        resultados[struct_idx].timeSwapSort = timeSwapSort;
        resultados[struct_idx].timeInsertionSort = timeInsertionSort;
        resultados[struct_idx].timeSelectionSort = timeSelectionSort;
        resultados[struct_idx].timeCocktailSort = timeCocktailSort;
        resultados[struct_idx].timeLinearSearch = timeLinearSearch;
        resultados[struct_idx].timeBinarySearch = timeBinarySearch;

        // Guardar en CSV
        fprintf(csv,
                "%d,%f,%f,%f,%f,%f,%f\n",
                resultados[struct_idx].n,
                resultados[struct_idx].timeSwapSort,
                resultados[struct_idx].timeInsertionSort,
                resultados[struct_idx].timeSelectionSort,
                resultados[struct_idx].timeCocktailSort,
                resultados[struct_idx].timeLinearSearch,
                resultados[struct_idx].timeBinarySearch
            );
        printf("\tEscrita informacion en CSV\n");
        struct_idx++;
    }
    fclose(csv);

    printf("\nDatos guardados en build/db/comparar_Algoritmos.csv\n");

    free(players);
    free(swapSortPlayers);
    free(insertionSortPlayers);
    free(selectionSortPlayers);
    free(cocktailShakerSortPlayers);
    free(linearSearchPlayers);
    free(binarySearchPlayers);
}