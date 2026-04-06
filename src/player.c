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
 * @brief Compara dos jugadores basandose en su ID.
 * * Realiza una comparacion numerica directa del campo `id`.
 * * @param p1 Puntero al primer jugador.
 * @param p2 Puntero al segundo jugador.
 * @return -1 si el ID de p1 es menor, 1 si es mayor, 0 si son iguales.
 */
int compare_id(const Player *p1, const Player *p2)
{
    // Verifica si el primer ID es estrictamente menor
    if (p1->id < p2->id) {
        return -1;
    }
    
    // Verifica si el primer ID es estrictamente mayor
    if (p1->id > p2->id) {
        return 1;
    }
    
    // Si no es menor ni mayor, los IDs son identicos
    return 0;
}

/**
 * @brief Compara dos jugadores alfabeticamente por su nombre.
 * * Utiliza la funcion estandar strcmp para determinar el orden lexicografico.
 * * @param p1 Puntero al primer jugador.
 * @param p2 Puntero al segundo jugador.
 * @return -1 si el nombre de p1 va antes, 1 si va despues, 0 si son iguales.
 */
int compare_name(const Player *p1, const Player *p2)
{
    int comparisonResult;
    
    // strcmp devuelve <0 si p1->name va antes, >0 si va despues, o 0 si son iguales
    comparisonResult = strcmp(p1->name, p2->name);

    if (comparisonResult < 0) {
        return -1;
    }
    
    if (comparisonResult > 0) {
        return 1;
    }
    
    return 0;
}

/**
 * @brief Compara dos jugadores alfabeticamente por el nombre de su equipo.
 * * Utiliza la funcion estandar strcmp para evaluar la cadena de caracteres del equipo.
 * * @param p1 Puntero al primer jugador.
 * @param p2 Puntero al segundo jugador.
 * @return -1 si el equipo de p1 va antes, 1 si va despues, 0 si son iguales.
 */
int compare_team(const Player *p1, const Player *p2)
{
    int comparisonResult;
    
    // Compara lexicograficamente los nombres de los equipos
    comparisonResult = strcmp(p1->team, p2->team);

    if (comparisonResult < 0) {
        return -1;
    }
    
    if (comparisonResult > 0) {
        return 1;
    }
    
    return 0;
}

/**
 * @brief Compara dos jugadores basandose en su puntuacion.
 * * Evalua los valores de punto flotante (double) del campo `score`.
 * * @param p1 Puntero al primer jugador.
 * @param p2 Puntero al segundo jugador.
 * @return -1 si la puntuacion de p1 es menor, 1 si es mayor, 0 si son iguales.
 */
int compare_score(const Player *p1, const Player *p2)
{
    // Compara si la puntuacion del primer jugador es mas baja
    if (p1->score < p2->score) {
        return -1;
    }
    
    // Compara si la puntuacion del primer jugador es mas alta
    if (p1->score > p2->score) {
        return 1;
    }
    
    // En caso de empate en la puntuacion
    return 0;
}

/**
 * @brief Compara dos jugadores basandose en la cantidad de competiciones.
 * * Compara los valores enteros del campo `competitions` jugadas por cada uno.
 * * @param p1 Puntero al primer jugador.
 * @param p2 Puntero al segundo jugador.
 * @return -1 si las competiciones de p1 son menores, 1 si son mayores, 0 si son iguales.
 */
int compare_competitions(const Player *p1, const Player *p2)
{
    // Verifica si el primer jugador ha participado en menos competiciones
    if (p1->competitions < p2->competitions) {
        return -1;
    }
    
    // Verifica si el primer jugador ha participado en mas competiciones
    if (p1->competitions > p2->competitions) {
        return 1;
    }
    
    // Si tienen la misma cantidad de competiciones
    return 0;
}


/**
 * @brief Imprime un arreglo de jugadores en consola.
 * 
 * @param players Arreglo de jugadores a imprimir.
 * @param n Tamanio del arreglo.
 */
void print_player_array(Player *players, int n) {
    // Se imprimen cabeceras
    printf(
		DARK_GRAY "|" RESET " "
		LIGHT_GRAY "%4s" RESET " "
		DARK_GRAY "|" RESET " "
		DARK_YELLOW "%10s" RESET " "
		DARK_GRAY "|" RESET " "
		DARK_BLUE "%7s" RESET " "
		DARK_GRAY "|" RESET " "
		DARK_GREEN "%4s" RESET " "
		DARK_GRAY "|" RESET " "
		PURPLE "%3s" RESET " "
		DARK_GRAY "|" RESET "\n",
		"ID", "NAME", "TEAM", "SCORE", "COMPS"
	);

    // Se imprimen los datos
    for (int i = 0; i < n; i++) {
        printf(
			DARK_GRAY "|" RESET " "
			WHITE "%4d" RESET " "
			DARK_GRAY "|" RESET " "
			YELLOW "%10s" RESET " "
			DARK_GRAY "|" RESET " "
			LIGHT_BLUE "%7s" RESET " "
			DARK_GRAY "|" RESET " "
			LIGHT_GREEN "%5.1f" RESET " "
			DARK_GRAY "|" RESET " "
			MAGENTA "%5d" RESET " "
			DARK_GRAY "|" RESET "\n",
			players[i].id, players[i].name, players[i].team, players[i].score, players[i].competitions
		);
    }
}