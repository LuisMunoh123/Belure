/**
 * @file player.h
 * @author Andres Barbosa, Milton Hernández, Iván Gallardo
 * @brief Definición de la estructura Player y prototipos de las funciones de comparación.
 */

#ifndef PLAYER_H
#define PLAYER_H

/**
 * @brief Estructura que representa a un jugador.
 * * Contiene toda la información básica necesaria para identificar
 * y evaluar el desempeño de un jugador dentro de las competiciones.
 */
typedef struct {
    int id;               /**< Identificador único numérico del jugador. */
    char name[10];        /**< Nombre del jugador (máximo 10 caracteres). */
    char team[50];        /**< Nombre del equipo al que pertenece el jugador. */
    double score;         /**< Puntuación general o calificación del jugador. */
    int competitions;     /**< Cantidad total de competiciones en las que ha participado. */
} Player;

/**
 * @brief Compara dos jugadores basándose en su ID.
 * * @param p1 Puntero al primer jugador a comparar.
 * @param p2 Puntero al segundo jugador a comparar.
 * @return -1 si el ID de p1 es menor, 1 si es mayor, 0 si son iguales.
 */
int compare_id(const Player *p1, const Player *p2);

/**
 * @brief Compara dos jugadores alfabéticamente por su nombre.
 * * @param p1 Puntero al primer jugador a comparar.
 * @param p2 Puntero al segundo jugador a comparar.
 * @return -1 si el nombre de p1 va antes alfabéticamente, 1 si va después, 0 si son iguales.
 */
int compare_name(const Player *p1, const Player *p2);

/**
 * @brief Compara dos jugadores alfabéticamente por su equipo.
 * * @param p1 Puntero al primer jugador a comparar.
 * @param p2 Puntero al segundo jugador a comparar.
 * @return -1 si el equipo de p1 va antes alfabéticamente, 1 si va después, 0 si son iguales.
 */
int compare_team(const Player *p1, const Player *p2);

/**
 * @brief Compara dos jugadores basándose en su puntuación (score).
 * * @param p1 Puntero al primer jugador a comparar.
 * @param p2 Puntero al segundo jugador a comparar.
 * @return -1 si la puntuación de p1 es menor, 1 si es mayor, 0 si son iguales.
 */
int compare_score(const Player *p1, const Player *p2);

/**
 * @brief Compara dos jugadores basándose en su número de competiciones.
 * * @param p1 Puntero al primer jugador a comparar.
 * @param p2 Puntero al segundo jugador a comparar.
 * @return -1 si las competiciones de p1 son menores, 1 si son mayores, 0 si son iguales.
 */
int compare_competitions(const Player *p1, const Player *p2);

#endif // PLAYER_H