/**
 * @file player.c
 * @author Andres Barbosa, Milton Hernandez, Ivan Gallardo
 * @brief Implementacion de las funciones de comparacion para la estructura Player.
 */

#include "player.h"

/**
 * @brief Intercambia dos jugadores
 * @param p1 puntero a jugador 1
 * @param p2 puntero a jugador 2
 */
void swap_player(Player *p1, Player *p2) {
	Player temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}

/**
 * @brief Compara si dos jugadores son exactamente iguales en todos sus campos.
 * * @param p1 Puntero al primer jugador.
 * @param p2 Puntero al segundo jugador.
 * @return true Si todos los campos son equivalentes.
 * @return false Si al menos un campo es distinto.
 */
bool are_players_equal(Player *p1, Player *p2) {
    // Validar que los punteros no sean nulos
    if (p1 == NULL || p2 == NULL) return false;

    // Comparar campos numericos y booleanos
    if (p1->id != p2->id) return false;
    if (p1->score != p2->score) return false;
    if (p1->competitions != p2->competitions) return false;
    if (p1->potatoe != p2->potatoe) return false;

    // Comparar arreglos de caracteres estaticos (name)
    if (strcmp(p1->name, p2->name) != 0) return false;

    // Comparar punteros a caracteres (team)
    // Manejo de casos donde uno o ambos punteros a team puedan ser NULL
    if (p1->team == NULL && p2->team != NULL) return false;
    if (p1->team != NULL && p2->team == NULL) return false;
    if (p1->team != NULL && p2->team != NULL) {
        if (strcmp(p1->team, p2->team) != 0) return false;
    }

    return true;
}

/**
 * @brief Compara dos jugadores basandose en su ID.
 * * Realiza una comparacion numerica directa del campo `id`.
 * * @param p1 Puntero al primer jugador.
 * @param p2 Puntero al segundo jugador.
 * @return -1 si el ID de p1 es menor, 1 si es mayor, 0 si son iguales.
 */
int compare_id(Player *p1, Player *p2) {
    return p1->id - p2->id;
}

/**
 * @brief Compara dos jugadores alfabeticamente por su nombre.
 * * Utiliza la funcion estandar strcmp para determinar el orden lexicografico.
 * * @param p1 Puntero al primer jugador.
 * @param p2 Puntero al segundo jugador.
 * @return -1 si el nombre de p1 va antes, 1 si va despues, 0 si son iguales.
 */
int compare_name(Player *p1, Player *p2) {
    return strcmp(p1->name, p2->name);
}

/**
 * @brief Compara dos jugadores alfabeticamente por el nombre de su equipo.
 * * Utiliza la funcion estandar strcmp para evaluar la cadena de caracteres del equipo.
 * * @param p1 Puntero al primer jugador.
 * @param p2 Puntero al segundo jugador.
 * @return -1 si el equipo de p1 va antes, 1 si va despues, 0 si son iguales.
 */
int compare_team(Player *p1, Player *p2) {
    // Proteccion en caso de que algun jugador no tenga equipo (NULL)
    if (p1->team == NULL && p2->team == NULL) return 0;
    if (p1->team == NULL) return -1;
    if (p2->team == NULL) return 1;
    return strcmp(p1->team, p2->team);
}

/**
 * @brief Compara dos jugadores basandose en su puntuacion.
 * * Evalua los valores de punto flotante (double) del campo `score`.
 * * @param p1 Puntero al primer jugador.
 * @param p2 Puntero al segundo jugador.
 * @return -1 si la puntuacion de p1 es menor, 1 si es mayor, 0 si son iguales.
 */
int compare_score(Player *p1, Player *p2) {
    if (p1->score > p2->score) return 1;
    if (p1->score < p2->score) return -1;
    return 0;
}
/**
 * @brief Compara dos jugadores basandose en la cantidad de competiciones.
 * * Compara los valores enteros del campo `competitions` jugadas por cada uno.
 * * @param p1 Puntero al primer jugador.
 * @param p2 Puntero al segundo jugador.
 * @return -1 si las competiciones de p1 son menores, 1 si son mayores, 0 si son iguales.
 */
int compare_competitions(Player *p1, Player *p2) {
    return p1->competitions - p2->competitions;
}
/**
 * @brief Imprime un arreglo de jugadores en consola.
 * 
 * @param players Arreglo de jugadores a imprimir.
 * @param n Tamanio del arreglo.
 */
/**
 * @brief Imprime el arreglo de jugadores en formato de tabla
 * * @param players Arreglo de jugadores
 * @param n Tamanho del arreglo
 */
void print_player_Array(Player *players, int n) {
    printf("\n");
    // Imprimir cabecera de la tabla (Sin Potatoe)
    printf("%-5s | %-10s | %-10s | %-6s | %-12s\n", 
           "ID", "Name", "Team", "Score", "Competitions");
    printf("-------------------------------------------------------\n");
    
    // Imprimir cada jugador (Sin Potatoe)
    for (int i = 0; i < n; i++) {
        printf("%-5d | %-10s | %-10s | %-6.1f | %-12d\n",
               players[i].id,
               players[i].name,
               (players[i].team != NULL) ? players[i].team : "N/A",
               players[i].score,
               players[i].competitions);
    }
    printf("\n");
}