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

// Aqui van unas palabras de Dios:
// ...
// ...
// ...
    // * Nota de autor: Milton dejo el visto y se demoro mas de 5 mins en responder.
    // Por lo que asumiremos que Dios es mudo.
