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

#define DEBUG 5 // Reducido a 5 para visualizar claramente los casos de prueba

int main() {
	// Variables de control
	int option = 0;
	int check;

	// UNICO ARREGLO DE JUGADORES CON LOS CASOS DE PRUEBA
	// - id: Mejor caso (Ordenado ascendentemente: 1, 2, 3, 4, 5)
	// - name: Peor caso (Ordenado descendentemente: Z -> V)
	// - competitions: Caso promedio (Desordenado aleatoriamente)
	Player players[DEBUG] = {
		{1, "Zeta",   "TeamA", 8.5, 15, false},
		{2, "Yara",   "TeamB", 4.2,  2, true},
		{3, "Xander", "TeamC", 9.1, 20, false},
		{4, "Wally",  "TeamA", 6.7,  8, true},
		{5, "Victor", "TeamB", 7.4, 12, false}
	};

	// Variables para la generacion de datos (Opcion 1)
	int n; 

	// Variables para ordenamiento (Opcion 2)
	int sort_option;
	int sort_criteria;

	// Variables para busqueda (Opcion 3)
	int search_option;
	int search_id;
	int result;

	srand(time(0));

	while (option != 4) {
		printf(RESET "\n===== MAIN MENU =====\n");
		printf(DARK_YELLOW "1)" YELLOW "Generate CSV\n");
		printf(DARK_BLUE "2)" LIGHT_BLUE "Sort array\n");
		printf(DARK_GREEN "3)" LIGHT_GREEN "Search value\n");
		printf(PURPLE "4)" MAGENTA "Exit\n");
		printf(DARK_GRAY"Choose an option: " RESET);
		scanf("%d", &option);

		while (check != 1 || option < 1 || option > 4) {
			printf(DARK_GRAY"Invalid option, try again: " RESET);
			while (getchar() != '\n');
			check = scanf("%d", &option);
		}

		if (option == 1) { // Opcion 1: Generar CSV
			printf(DARK_YELLOW "\nType the number of players to generate: " YELLOW);
			scanf("%d", &n);
			generate_csv(n);
		}

		else if (option == 2) { // Opcion 2: Ordenar arreglo
			printf(LIGHT_BLUE "\nOriginal array:\n" RESET);
			print_player_Array(players, DEBUG); 

			printf(DARK_BLUE "\nChoose a sorting algorithm:\n" LIGHT_BLUE);
			printf("1) Swap Sort\n");
			printf("2) Insertion Sort\n");
			printf("3) Selection Sort\n");
			printf("4) Cocktail Shaker Sort\n");
			printf(DARK_BLUE "Option: " LIGHT_BLUE);
			scanf("%d", &sort_option);

			// NUEVO MENU: Elegir el campo a ordenar
			printf(DARK_BLUE "\nChoose field to sort by (Ascending Order):\n" LIGHT_BLUE);
			printf("1) ID\n");
			printf("2) Name\n");
			printf("3) Team\n");
			printf("4) Score\n");
			printf("5) Competitions\n");
			printf(DARK_BLUE "Option: " LIGHT_BLUE);
			scanf("%d", &sort_criteria);

			// Asignar el comparador segun la eleccion del usuario
			int (*comp_ptr)(Player*, Player*) = NULL;
			if (sort_criteria == 1) comp_ptr = compare_id;
			else if (sort_criteria == 2) comp_ptr = compare_name;
			else if (sort_criteria == 3) comp_ptr = compare_team;
			else if (sort_criteria == 4) comp_ptr = compare_score;
			else if (sort_criteria == 5) comp_ptr = compare_competitions;
			else {
				printf(RESET BG_RED "Invalid criteria option.\n" RESET);
				continue;
			}

			// Ejecutar el ordenamiento
			if (sort_option == 1) {
				swap_sort(players, DEBUG, comp_ptr);
			} else if (sort_option == 2) {
				insertion_sort(players, DEBUG, comp_ptr);
			} else if (sort_option == 3) {
				selection_sort(players, DEBUG, comp_ptr);
			} else if (sort_option == 4) {
				cocktail_shaker_sort(players, DEBUG, comp_ptr);
			} else {
				printf(RESET BG_RED "Invalid sorting option.\n" RESET);
				continue;
			}

			printf(LIGHT_BLUE "\nSorted array (Ascending):\n" RESET);
			print_player_Array(players, DEBUG);
		}

		else if (option == 3) { // Opcion 3: Buscar valor
			// Quitamos el RESET de esta linea para que la tabla herede el color
			printf(LIGHT_GREEN"\nCurrent array:\n");
			print_player_Array(players, DEBUG);
			printf(RESET); // Reseteamos despues de la tabla

			printf(DARK_GREEN"\nChoose a searching algorithm:\n"LIGHT_GREEN);
			printf("1) Linear Search (By ID)\n");
			printf("2) Binary Search (By ID)\n");
			printf(DARK_GREEN"Option: " LIGHT_GREEN);
			scanf("%d", &search_option);

			if (search_option == 1 || search_option == 2) {
				printf("Type the ID to search: ");
				scanf("%d", &search_id);
				
				Player target = {search_id, "", "", 0.0, 0, false};

				if (search_option == 1) {
					result = linear_search(players, DEBUG, &target, compare_id);

					if (result == DEBUG) {
						printf(BG_RED "Player with ID %d was not found." RESET "\n", search_id);
					} else {
						// RESET antes del \n para evitar que el fondo verde manche toda la pantalla
						printf(BG_GREEN "Player found at index %d via Linear Search:" RESET "\n", result+1);
						
						// Imprimimos la cabecera sin Potatoe y forzando color LIGHT_GREEN
						printf(LIGHT_GREEN "%-5s | %-10s | %-10s | %-6s | %-12s\n", "ID", "Name", "Team", "Score", "Competitions");
						printf("-------------------------------------------------------\n");
						printf("%-5d | %-10s | %-10s | %-6.1f | %-12d\n" RESET,
							players[result].id, players[result].name, players[result].team,
							players[result].score, players[result].competitions);
					}
				} 
				else if (search_option == 2) {
					insertion_sort(players, DEBUG, compare_id);

					printf(LIGHT_GREEN "\nOrdered array (by ID) used for binary search:\n");
					print_player_Array(players, DEBUG);
					printf(RESET); // Reseteamos despues de la tabla

					result = binary_search(players, 0, DEBUG - 1, &target);

					if (result == -1) {
						printf(BG_RED "Player with ID %d was not found." RESET "\n", search_id);
					} else {
						// RESET antes del \n para evitar que el fondo verde manche toda la pantalla
						printf(BG_GREEN "Player found at index %d via Binary Search:" RESET "\n", result+1);
						
						// Imprimimos la cabecera sin Potatoe y forzando color LIGHT_GREEN
						printf(LIGHT_GREEN "%-5s | %-10s | %-10s | %-6s | %-12s\n", "ID", "Name", "Team", "Score", "Competitions");
						printf("-------------------------------------------------------\n");
						printf("%-5d | %-10s | %-10s | %-6.1f | %-12d\n" RESET,
							players[result].id, players[result].name, players[result].team,
							players[result].score, players[result].competitions);
					}
				}
			}
			else {
				printf("Invalid searching option.\n");
			}
		}

		else if (option == 4) { // Opcion 4: Salir
			printf(PURPLE "\nExiting program...\n" RESET);
		}

		else {
			printf("\nInvalid option.\n");
		}
	}

	return 0;
}