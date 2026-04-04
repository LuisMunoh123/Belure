/**
 * @file main.c
 * @author Andres Barbosa, Milton Hernández, Iván Gallardo
 * @brief Punto de inicio del programa y pruebas de ordenamiento, generacion y busqueda.
 */

 /* main.c de Andres
#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "player.h"
*/

/**
 * @brief Evalúa el resultado de una comparación e imprime el veredicto en pantalla.
 * * Esta función toma el valor de retorno de las funciones de comparación (-1, 0, 1)
 * y muestra un mensaje formateado en la consola indicando la relación entre los 
 * dos elementos evaluados. También valida que los punteros de nombre no sean nulos.
 *
 * @param result Resultado numérico de la función de comparación previa.
 * @param nameOne Nombre del primer jugador evaluado.
 * @param nameTwo Nombre del segundo jugador evaluado.
 * @param field Etiqueta descriptiva del campo que se está comparando (ej. "ID", "Name").
 */

/*
void evaluate_and_show(int result, const char* nameOne, const char* nameTwo, const char* field)
{
    if (nameOne == NULL || nameTwo == NULL) {
        // Usamos la función de manejo de errores definida en tu errors.h
        print_error(100, "Puntero nulo detectado", NULL);
        return; 
    }

    if (result == -1) {
        printf("[%s] The value of %s is LESS than %s\n", field, nameOne, nameTwo);
        return;
    } 
    
    if (result == 1) {
        printf("[%s] The value of %s is GREATER than %s\n", field, nameOne, nameTwo);
        return;
    } 
    
    printf("[%s] The value of %s is EQUAL to %s\n", field, nameOne, nameTwo);
}    
    */

/**
 * @brief Función principal del programa.
 * * Crea un conjunto de jugadores de prueba estáticos y ejecuta una serie de 
 * comparaciones directas (por ID, nombre, equipo, puntuación y competiciones)
 * para validar la correcta implementación de las funciones del archivo player.c.
 * Posteriormente simula un escenario de error abriendo un archivo inexistente.
 *
 * @return 0 si la ejecución finaliza correctamente, 1 si ocurre algún error.
 */

/*
int main()
{
    // Variables no iteradoras declaradas por separado (camelCase)
    Player playerOne = {1, "Juan", "Lions", 8.5, 50};
    Player playerTwo = {2, "Ana", "Tigers", 9.2, 30};
    Player playerThree = {3, "Carlos", "Eagles", 8.5, 75};
    Player playerFour = {4, "Juan", "Pumas", 7.0, 50};
    Player playerFive = {5, "Beto", "Lions", 10.0, 99};

    printf("=== DIRECT COMPARISON TESTS ===\n\n");

    printf("--- ID Tests ---\n");
    evaluate_and_show(compare_id(&playerOne, &playerTwo), playerOne.name, playerTwo.name, "ID");
    
    printf("\n--- Name Tests ---\n");
    evaluate_and_show(compare_name(&playerOne, &playerTwo), playerOne.name, playerTwo.name, "Name");
    evaluate_and_show(compare_name(&playerOne, &playerFour), playerOne.name, playerFour.name, "Name");

    printf("\n--- Team Tests ---\n");
    evaluate_and_show(compare_team(&playerTwo, &playerThree), playerTwo.name, playerThree.name, "Team");
    evaluate_and_show(compare_team(&playerOne, &playerFive), playerOne.name, playerFive.name, "Team");

    printf("\n--- Score Tests ---\n");
    evaluate_and_show(compare_score(&playerTwo, &playerFive), playerTwo.name, playerFive.name, "Score");
    evaluate_and_show(compare_score(&playerOne, &playerThree), playerOne.name, playerThree.name, "Score");

    printf("\n--- Competitions Tests ---\n");
    evaluate_and_show(compare_competitions(&playerThree, &playerFive), playerThree.name, playerFive.name, "Competitions");
    evaluate_and_show(compare_competitions(&playerOne, &playerFour), playerOne.name, playerFour.name, "Competitions");

    printf("\n=== ORIGINAL MAIN CODE EXECUTION ===\n");

    // Apertura de un archivo inexistente para mostrar el error (De tu plantilla original)
    FILE *fp = fopen("inexistente.txt", "r");
    if (fp == NULL) {
        print_error(100, "inexistente.txt", NULL);
        return 1;
    }

    return 0;
}
    */

// main.c de Ivan
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "generator.h"
#include "sorting.h"
#include "searching.h"
#include "utilities.h"

#define DEBUG 20

int main() {
    int option = 0;

    int best[]  = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    int worst[] = {20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
    int shuffled[] = {7,5,1,9,3,8,4,2,6,10,20,18,12,14,16,11,19,17,15,13};

    int V[DEBUG];

    srand(time(0));

    while (option != 4) {
        printf(RESET "\n===== MAIN MENU =====\n");
        printf(DARK_YELLOW "1)" YELLOW "Generate CSV\n");
        printf(DARK_BLUE "2)" LIGHT_BLUE "Sort array\n");
        printf(DARK_GREEN "3)" LIGHT_GREEN "Search value\n");
        printf(PURPLE "4)" MAGENTA "Exit\n");
        printf(DARK_GRAY"Choose an option: " RESET);
        scanf("%d", &option);

        if (option == 1) {
            int n;

            printf(DARK_YELLOW "\nType the number of players to generate: ");
            scanf("%d", &n);

            generate_csv(n);
        }

        else if (option == 2) {
            int array_option;
            int sort_option;

            printf(DARK_BLUE "\nChoose an array:\n" LIGHT_BLUE);
            printf("1) Best case\n");
            printf("2) Worst case\n");
            printf("3) Shuffled case\n");
            printf(DARK_BLUE "Option: ");
            scanf("%d", &array_option);

            if (array_option == 1) {
                for (int i = 0; i < DEBUG; i++) {
                    V[i] = best[i];
                }
            } else if (array_option == 2) {
                for (int i = 0; i < DEBUG; i++) {
                    V[i] = worst[i];
                }
            } else if (array_option == 3) {
                for (int i = 0; i < DEBUG; i++) {
                    V[i] = shuffled[i];
                }
            } else {
                printf("Invalid array option.\n");
                continue;
            }

            printf(LIGHT_BLUE "\nOriginal array:\n");
            for (int i = 0; i < DEBUG; i++) {
                printf("%d ", V[i]);
            }
            printf("\n");

            printf(DARK_BLUE "\nChoose a sorting algorithm:\n" LIGHT_BLUE);
            printf("1) Swap Sort\n");
            printf("2) Insertion Sort\n");
            printf("3) Selection Sort\n");
            printf("4) Cocktail Shaker Sort\n");
            printf(DARK_BLUE "Option: ");
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
            for (int i = 0; i < DEBUG; i++) {
                printf("%d ", V[i]);
            }
            printf("\n");
        }

        else if (option == 3) {
            int array_option;
            int search_option;
            int x;
            int result;

            printf(DARK_GREEN "\nChoose an array:\n" LIGHT_GREEN);
            printf("1) Best case\n");
            printf("2) Worst case\n");
            printf("3) Shuffled case\n");
            printf(DARK_GREEN "Option: ");
            scanf("%d", &array_option);

            if (array_option == 1) {
                for (int i = 0; i < DEBUG; i++) {
                    V[i] = best[i];
                }
            } else if (array_option == 2) {
                for (int i = 0; i < DEBUG; i++) {
                    V[i] = worst[i];
                }
            } else if (array_option == 3) {
                for (int i = 0; i < DEBUG; i++) {
                    V[i] = shuffled[i];
                }
            } else {
                printf("Invalid array option.\n");
                continue;
            }

            printf(LIGHT_GREEN"\nCurrent array:\n");
            for (int i = 0; i < DEBUG; i++) {
                printf("%d ", V[i]);
            }
            printf("\n");

            printf(DARK_GREEN"\nChoose a searching algorithm:\n"LIGHT_GREEN);
            printf("1) Linear Search\n");
            printf("2) Binary Search\n");
            printf(DARK_GREEN"Option: ");
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
                insertion_sort(V, DEBUG);

                printf(LIGHT_GREEN "\nOrdered array used for binary search:\n");
                for (int i = 0; i < DEBUG; i++) {
                    printf("%d ", V[i]);
                }
                printf("\n");

                result = binary_search(V, 0, DEBUG - 1, x);

                if (result == -1) {
                    printf(RESET BG_RED "Value %d was not found.\n", x);
                } else {
                    printf(RESET BG_GREEN "Value %d found at index %d.\n", x, result);
                }
            }

            else {
                printf("Invalid searching option.\n");
            }
        }

        else if (option == 4) {
            printf(PURPLE "\nExiting program...\n");
        }

        else {
            printf("\nInvalid option.\n");
        }
    }

	printf(RESET);
    return 0;
}