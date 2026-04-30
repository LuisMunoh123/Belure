/**
 * @file generator.h
 * @author Andres Barbosa, Milton Hernandez, Ivan Gallardo, Franco Aguilar
 * @brief Funcion y utilidades de generacion
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "utilities.h"
#include "errors.h"
#include "player.h"

int generate_csv(int n, GenerationType case_type);
Player* load_players(char* file, int *out_n);

#endif