/**
 * @file sorting.h
 * @author Andres Barbosa, Milton Hernandez, Ivan Gallardo
 * @brief Funciones de ordenamiento
 */

#ifndef SORTING_H
#define SORTING_H

#include <stdio.h>
#include "player.h"

/**
 * @file sorting.h
 * @author Andres Barbosa, Milton Hernandez, Ivan Gallardo
 * @brief Funciones de ordenamiento
 */
void swap_sort(Player V[], int n, int (*comp_f)(Player *, Player *));
void insertion_sort(Player V[], int n, int (*comp_f)(Player *, Player *));
void selection_sort(Player V[], int n, int (*comp_f)(Player *, Player *));
void cocktail_shaker_sort(Player V[], int n, int (*comp_f)(Player *, Player *));

#endif // SORTING_H

//a q kle peptega