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

#define DEBUG 20 // Tamanio del arreglo de prueba

void print_array(int *V, int n);

int main() {

	// Variables de control
	int option = 0;
	int check;

	// Arreglos
	int V[DEBUG] = {7,5,1,9,3,8,4,2,6,10,20,18,12,14,16,11,19,17,15,13};

	// Variables para la generacion de datos (Opcion 1)
	int n; 

	// Variables para ordenamiento (Opcion 2)
	int sort_option;

	// Variables para busqueda (Opcion 3)
	int search_option;
	int x;
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
			printf(LIGHT_BLUE "\nOriginal array:\n");
			print_array(V, DEBUG);

			printf(DARK_BLUE "\nChoose a sorting algorithm:\n" LIGHT_BLUE);
			printf("1) Swap Sort\n");
			printf("2) Insertion Sort\n");
			printf("3) Selection Sort\n");
			printf("4) Cocktail Shaker Sort\n");
			printf(DARK_BLUE "Option: " LIGHT_BLUE);
			scanf("%d", &sort_option);

			if (sort_option == 1) {
				swap_sort(V, DEBUG);
			} else if (sort_option == 2) {
				insertion_sort(V, DEBUG);
			} else if (sort_option == 3) {
				selection_sort(V, DEBUG);
			} else if (sort_option == 4) {
				cocktail_shaker_sort(V, DEBUG);
			} else {
				printf("Invalid sorting option.\n");
				continue;
			}

			printf(LIGHT_BLUE "\nSorted array:\n");
			print_array(V, DEBUG);
		}

		else if (option == 3) { // Opcion 3: Buscar valor
			printf(LIGHT_GREEN"\nCurrent array:\n");
			print_array(V, DEBUG);

			printf(DARK_GREEN"\nChoose a searching algorithm:\n"LIGHT_GREEN);
			printf("1) Linear Search\n");
			printf("2) Binary Search\n");
			printf(DARK_GREEN"Option: " LIGHT_GREEN);
			scanf("%d", &search_option);

			printf("Type the value to search: ");
			scanf("%d", &x);

			if (search_option == 1) {
				result = linear_search(V, DEBUG, x);

				if (result == DEBUG) {
					printf(RESET BG_RED "Value %d was not found.\n", x);
				} else {
					printf(RESET BG_GREEN "Value %d found at index %d.\n", x, result);
				}
			}

			else if (search_option == 2) {
				insertion_sort(V, DEBUG); // Ordenamos con el arreglo previo a la busqueda

				printf(LIGHT_GREEN "\nOrdered array used for binary search:\n");
				print_array(V, DEBUG);

				result = binary_search(V, 0, DEBUG - 1, x);

				if (result == -1) {
					printf(RESET BG_RED "Value %d was not found.\n", x);
				} else {
					printf(RESET BG_GREEN "Value %d found at index %d (of the sortered array).\n", x, result);
				}
			}

			else {
				printf("Invalid searching option.\n");
			}
		}

		else if (option == 4) { // Opcion 4: Salir
			printf(PURPLE "\nExiting program...\n");
		}

		else {
			printf("\nInvalid option.\n");
		}
	}

	printf(RESET);
	return 0;
}


void print_array(int *V, int n) {
	printf("[ ");
	for (int i = 0; i < n; i++) {
		printf("%d ", V[i]);
	}
	printf("]\n");
	printf("\n");
}