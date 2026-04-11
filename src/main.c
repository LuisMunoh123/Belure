/**
 * @file main.c
 * @author Andres Barbosa, Milton Hernandez, Ivan Gallardo
 * @brief Punto de inicio del programa y pruebas de ordenamiento, generacion y busqueda.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "generator.h"
#include "sorting.h"
#include "searching.h"
#include "utilities.h"
#include "player.h" 
#include "generate_exec_times.h"

int main() {
	// Variables de control
	int option = 0;
	int sortType = 0;
	int check = 1;

	Player* players = NULL;
	int n; // Numero de jugadores

	// Variables para ordenamiento (Opcion 2)
	int sortOption;
	int sortCriteria;

	// Variables para busqueda (Opcion 3)
	int searchOption;
	int searchId;
	int result;

	srand(time(0));

	printf(RESET "\n===== MAIN MENU =====\n");
	printf(DARK_YELLOW "1)" YELLOW " Generate CSV\n");
	printf(DARK_BLUE "2)" LIGHT_BLUE " Sort array\n");
	printf(DARK_GREEN "3)" LIGHT_GREEN " Search value\n");
	printf(PURPLE "4)" MAGENTA " Run experiment\n");
	printf(DARK_RED "5)" LIGHT_RED " Exit\n");
	printf(DARK_GRAY"Choose an option: " RESET);
	
	check =	scanf("%d", &option);
	while (check != 1 || option < 1 || option > 5) {
		printf(DARK_GRAY"Invalid option, try again: " RESET);
		while (getchar() != '\n');
		check = scanf("%d", &option);
	}

	if (option == 1) { // Opcion 1: Generar CSV
		printf(DARK_YELLOW "\nType the number of players to generate: " YELLOW);
		check = scanf("%d", &n);
		while (check != 1 || n < 0) {
			printf(DARK_GRAY"Invalid number, try again: " RESET);
			while (getchar() != '\n');
			check = scanf("%d", &n);
		}

		printf("\n" DARK_YELLOW "Choose the case type:" RESET "\n");
		printf(DARK_YELLOW"1)" YELLOW" Best case\n");
		printf(DARK_YELLOW"2)" YELLOW" Worst case\n");
		printf(DARK_YELLOW"3)" YELLOW" Average case\n");
		printf(DARK_YELLOW"Option: " YELLOW);
		check = scanf("%d", &sortType);
		while (check != 1 || n < 0) {
			printf(DARK_GRAY"Invalid option, try again: " RESET);
			while (getchar() != '\n');
			check = scanf("%d", &sortType);
		}

		if (sortType == 1) {
			printf(YELLOW "Generating BEST case...\n");
		} else if (sortType == 2) {
			printf(YELLOW "Generating WORST case...\n");
		} else {
			printf(YELLOW "Generating AVERAGE case...\n");
		}

		generate_csv(n, sortType);
	}

	else if (option == 2) { // Opcion 2: Ordenar arreglo}
		// Cargamos el arreglo de jugadores
		if ((players = load_players("build/db/players.csv", &n)) == NULL) {
			return 1;
		}

		printf(LIGHT_BLUE "\nOriginal array:\n" RESET);
		print_player_array(players, n);

		printf(DARK_BLUE "\nChoose a sorting algorithm:\n" LIGHT_BLUE);
		printf("1) Swap Sort\n");
		printf("2) Insertion Sort\n");
		printf("3) Selection Sort\n");
		printf("4) Cocktail Shaker Sort\n");
		printf(DARK_BLUE "Option: " LIGHT_BLUE);
		
		check = scanf("%d", &sortOption);
		while (check != 1 || sortOption < 1 || sortOption > 4) {
			printf(DARK_GRAY"Invalid option, try again: " RESET);
			while (getchar() != '\n');
			check = scanf("%d", &sortOption);
		}

		// NUEVO MENU: Elegir el campo a ordenar
		printf(DARK_BLUE "\nChoose field to sort by (Ascending Order):\n" LIGHT_BLUE);
		printf("1) ID\n");
		printf("2) Name\n");
		printf("3) Team\n");
		printf("4) Score\n");
		printf("5) Competitions\n");
		printf(DARK_BLUE "Option: " LIGHT_BLUE);

		check = scanf("%d", &sortCriteria);
		while (check != 1 || sortCriteria < 1 || sortCriteria > 5) {
			printf(DARK_GRAY"Invalid option, try again: " RESET);
			while (getchar() != '\n');
			check = scanf("%d", &sortCriteria);
		}

		// Asignar el comparador segun la eleccion del usuario
		int (*comp_ptr)(Player*, Player*) = NULL;
		if (sortCriteria == 1) comp_ptr = compare_id;
		else if (sortCriteria == 2) comp_ptr = compare_name;
		else if (sortCriteria == 3) comp_ptr = compare_team;
		else if (sortCriteria == 4) comp_ptr = compare_score;
		else if (sortCriteria == 5) comp_ptr = compare_competitions;
		else {
			printf(RESET BG_RED "Invalid criteria option.\n" RESET);
		}

		// Ejecutar el ordenamiento
		if (sortOption == 1) {
			swap_sort(players, n, comp_ptr);
		} else if (sortOption == 2) {
			insertion_sort(players, n, comp_ptr);
		} else if (sortOption == 3) {
			selection_sort(players, n, comp_ptr);
		} else if (sortOption == 4) {
			cocktail_shaker_sort(players, n, comp_ptr);
		} else {
			printf(RESET BG_RED "Invalid sorting option.\n" RESET);
		}

		printf(LIGHT_BLUE "\nSorted array (Ascending):\n" RESET);
		print_player_array(players, n);
		free(players);
	}

	else if (option == 3) { // Opcion 3: Buscar valor
		// Cargamos el arreglo de jugadores
		if ((players = load_players("build/db/players.csv", &n)) == NULL) {
			return 1;
		}
	
		// Quitamos el RESET de esta linea para que la tabla herede el color
		printf(LIGHT_GREEN"\nCurrent array:\n");
		print_player_array(players, n);
		printf(RESET); // Reseteamos despues de la tabla

		printf(DARK_GREEN"\nChoose a searching algorithm:\n"LIGHT_GREEN);
		printf("1) Linear Search (By ID)\n");
		printf("2) Binary Search (By ID)\n");
		printf(DARK_GREEN"Option: " LIGHT_GREEN);
		check = scanf("%d", &searchOption);
		while (check != 1 || searchOption < 1 || searchOption > 2) {
			printf(DARK_GRAY"Invalid option, try again: " RESET);
			while (getchar() != '\n');
			check = scanf("%d", &searchOption);
		}

		if (searchOption == 1 || searchOption == 2) {
			printf("Type the ID to search: ");
			check = scanf("%d", &searchId);
			while (check != 1) {
				printf(DARK_GRAY"Invalid ID, try again: " RESET);
				while (getchar() != '\n');
				check = scanf("%d", &searchId);
			}
			
			Player target = {searchId, "", "", 0.0, 0, false};

			if (searchOption == 1) {
				result = linear_search(players, n, &target, compare_id);

				if (result == n) {
					printf(BG_RED "Player with ID %d was not found." RESET "\n\n", searchId);
				} else {
					printf(BG_GREEN "Player found at index %d via Linear Search:" RESET "\n\n", result+1);
					print_player(&players[result]);
				}
			} 
			else if (searchOption == 2) {
				cocktail_shaker_sort(players, n, compare_id);

				printf(LIGHT_GREEN "\nOrdered array (by ID) used for binary search:\n");
				print_player_array(players, n);
				printf(RESET); // Reseteamos despues de la tabla

				result = binary_search(players, 0, n - 1, &target);

				if (result == -1) {
					printf(BG_RED "Player with ID %d was not found." RESET "\n\n", searchId);
				} else {
					printf(BG_GREEN "Player found at index %d via Binary Search:" RESET "\n\n", result+1);
					print_player(&players[result]);
				}
			}
		}
		else {
			printf("Invalid searching option.\n");
		}

		free(players);
	}

	else if (option == 4) { // Opcion 4: Ejecutar experimento
		run_experiment();
	}

	else if (option == 5) { // Opcion 5: Salir
		printf(DARK_RED "\nExiting program...\n" RESET);
	}

	return 0;
}