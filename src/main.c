/**
 * @file main.c
 * @author Andres Barbosa, Milton Hernández, Iván Gallardo
 * @brief Punto de inicio del programa y pruebas de comparación de jugadores.
 */

#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "player.h"

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

/**
 * @brief Función principal del programa.
 * * Crea un conjunto de jugadores de prueba estáticos y ejecuta una serie de 
 * comparaciones directas (por ID, nombre, equipo, puntuación y competiciones)
 * para validar la correcta implementación de las funciones del archivo player.c.
 * Posteriormente simula un escenario de error abriendo un archivo inexistente.
 *
 * @return 0 si la ejecución finaliza correctamente, 1 si ocurre algún error.
 */
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