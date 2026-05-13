/**
 * @file selection.c
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Selection-related functions.
 */

#include"selection.h"

/**
 * @brief Finds the k-th element in an array of players.
 * 
 * @param V Array of players.
 * @param left First index in the array.
 * @param right Last index in the array.
 * @param k Index of the target element.
 * @param comp_f Comparison function (-1 if a < b, 1 if a > b, 0 if a = b).
 * @return Player* Pointer to the k-th element of V.
 */
Player quick_select(Player V[], int left, int right, int k, int (*comp_f)(Player *, Player *))
{
	if (left == right) {
		return V[left];
	}
	
	// Calculamos el pivote como mediana de tres y pasamos este valor a la particion Lomuto
	int pivot = lomuto_partition(V, left, right, median_of_three(V, left, right, comp_f), comp_f);

	if (k == pivot) { // El pivote correspondia al k-ésimo elemento
		return V[pivot];
	}
	else if (k < pivot) { // Buscamos el k-ésimo elemento en la izquierda
		return quick_select(V, left, pivot - 1, k, comp_f);
	}
	else { // Buscamos el k-ésimo elemento en la derecha
		return quick_select(V, pivot + 1, right, k, comp_f);
	}
}