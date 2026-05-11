/**
 * @file sorting.h
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Funciones de ordenamiento
 */

#ifndef SORTING_H
#define SORTING_H

#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "utilities.h"

void swap_sort(Player V[], int n, int (*comp_f)(Player *, Player *));
void insertion_sort(Player V[], int n, int (*comp_f)(Player *, Player *));
void selection_sort(Player V[], int n, int (*comp_f)(Player *, Player *));
void cocktail_shaker_sort(Player V[], int n, int (*comp_f)(Player *, Player *));
void merge_sort_classic(Player V[], int left, int right, int (*comp_f)(Player *, Player *));
void merge_sort_optimized(Player V[], int left, int right, int threshold, int (*comp_f)(Player *, Player *));
void quick_sort(Player V[], int left, int right, int (*comp_f)(Player *, Player *));
int lomuto_partition(Player V[], int left, int right, int pivot, int (*comp_f)(Player *, Player *));

#endif // SORTING_H
