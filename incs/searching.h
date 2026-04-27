/**
 * @file searching.h
 * @author Andres Barbosa, Milton Hernandez, Ivan Gallardo
 * @brief Funciones de busqueda
 */
#ifndef SEARCHING_H
#define SEARCHING_H

#include <stdio.h>
#include "player.h"

int linear_search(Player V[], int n, Player *x,int (*comp_f)(Player *, Player *));
int binary_search(Player V[], int beg, int end, Player *x);

#endif