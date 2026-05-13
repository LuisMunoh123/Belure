/**
 * @file searching.h
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Search functions.
 */
#ifndef SEARCHING_H
#define SEARCHING_H

#include <stdio.h>
#include "player.h"

int linear_search(Player V[], int n, Player *x, int (*comp_f)(Player *, Player *));
int binary_search_iterative(Player V[], int beg, int end, Player *x, int (*comp_f)(Player *, Player *));
int binary_search_recursive(Player V[], int beg, int end, Player *x, int (*comp_f)(Player *, Player *));
int binary_search_first(Player V[], int beg, int end, Player *x, int (*comp_f)(Player *, Player *));
int binary_search_last(Player V[], int beg, int end, Player *x, int (*comp_f)(Player *, Player *));
void binary_search_range(Player V[], int beg, int end, Player *x, int (*comp_f)(Player *, Player *), int *first, int *last);
int binary_search_lower_bound(Player V[], int beg, int end, Player *x, int (*comp_f)(Player *, Player *));
int binary_search_upper_bound(Player V[], int beg, int end, Player *x, int (*comp_f)(Player *, Player *));
int exponential_search(Player V[], int n, Player *x, int (*comp_f)(Player *, Player *));
int interpolation_search(Player V[], int beg, int end, int target_id);

#endif
