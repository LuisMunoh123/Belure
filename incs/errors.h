/**
 * @file errors.h
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Funciones para manejo de errores (errors.c)
 */
#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "player.h"
#include "utilities.h"

void print_error(int num, char *target, char *obs);

#endif