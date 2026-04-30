/**
 * @file selection.h
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Funciones de Seleccion
 */
#ifndef SELECTION_H
#define SELECTION_H

#include <stdio.h>
#include "player.h"
#include "sorting.h"

Player quick_select(Player V[], int left, int right, int k, int (*comp_f)(Player *, Player *));

#endif