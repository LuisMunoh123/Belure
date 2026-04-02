/**
 * @file player.c
 * @author Andres Barbosa, Milton Hernández, Iván Gallardo
 * @brief Implementación de las funciones de comparación para la estructura Player.
 */

#include "player.h"
#include <string.h>

/**
 * @brief Compara dos jugadores basándose en su ID.
 * * Realiza una comparación numérica directa del campo `id`.
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
    
    // Si no es menor ni mayor, los IDs son idénticos
    return 0;
}

/**
 * @brief Compara dos jugadores alfabéticamente por su nombre.
 * * Utiliza la función estándar strcmp para determinar el orden lexicográfico.
 * * @param p1 Puntero al primer jugador.
 * @param p2 Puntero al segundo jugador.
 * @return -1 si el nombre de p1 va antes, 1 si va después, 0 si son iguales.
 */
int compare_name(const Player *p1, const Player *p2)
{
    int comparisonResult;
    
    // strcmp devuelve <0 si p1->name va antes, >0 si va después, o 0 si son iguales
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
 * @brief Compara dos jugadores alfabéticamente por el nombre de su equipo.
 * * Utiliza la función estándar strcmp para evaluar la cadena de caracteres del equipo.
 * * @param p1 Puntero al primer jugador.
 * @param p2 Puntero al segundo jugador.
 * @return -1 si el equipo de p1 va antes, 1 si va después, 0 si son iguales.
 */
int compare_team(const Player *p1, const Player *p2)
{
    int comparisonResult;
    
    // Compara lexicográficamente los nombres de los equipos
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
 * @brief Compara dos jugadores basándose en su puntuación.
 * * Evalúa los valores de punto flotante (double) del campo `score`.
 * * @param p1 Puntero al primer jugador.
 * @param p2 Puntero al segundo jugador.
 * @return -1 si la puntuación de p1 es menor, 1 si es mayor, 0 si son iguales.
 */
int compare_score(const Player *p1, const Player *p2)
{
    // Compara si la puntuación del primer jugador es más baja
    if (p1->score < p2->score) {
        return -1;
    }
    
    // Compara si la puntuación del primer jugador es más alta
    if (p1->score > p2->score) {
        return 1;
    }
    
    // En caso de empate en la puntuación
    return 0;
}

/**
 * @brief Compara dos jugadores basándose en la cantidad de competiciones.
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
    
    // Verifica si el primer jugador ha participado en más competiciones
    if (p1->competitions > p2->competitions) {
        return 1;
    }
    
    // Si tienen la misma cantidad de competiciones
    return 0;
}