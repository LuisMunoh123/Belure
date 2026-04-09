/**
 * @file player.h
 * @author Andres Barbosa, Milton Hernandez, Ivan Gallardo
 * @brief Definicion de la estructura Player y prototipos de las funciones de comparacion.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <string.h>
#include <stdbool.h>

#include "utilities.h"

#define MIN_NAME_LENGTH 3
#define MAX_NAME_LENGTH 10

/**
 * @brief Estructura que representa a un jugador.
 * 
 */
typedef struct {
    
    int id;                       // Identificador numerico unico del jugador.
    char name[MAX_NAME_LENGTH+1]; // Nombre del jugador
    char *team;                   // Nombre del equipo al que pertenece el jugador.
    float score;                  // Puntuacion general o calificacion del jugador.
    int competitions;             // Cantidad total de competiciones en las que ha participado.
    bool potatoe;                 // Se usa para indicar si el jugador potatoe

} Player;

void swap_player(Player *p1, Player *p2);
int compare_id(Player *p1, Player *p2);
int compare_name(Player *p1, Player *p2);
int compare_team(Player *p1, Player *p2);
int compare_score(Player *p1, Player *p2);
int compare_competitions(Player *p1, Player *p2);
void print_player_Array(Player *players, int n);
bool are_players_equal(Player *p1, Player *p2);


#endif // PLAYER_H
// Dios es mudo pero a su vez es bastante sabio