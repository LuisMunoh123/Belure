/**
 * @file searching.c
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Funciones de busqueda
 */

#include"selection.h"

/**
 * @brief Funcion para buscar el k-ésimo elemento de un arreglo de Players
 * 
 * @param V Arreglo de Players
 * @param left Primer indice del arreglo
 * @param right Ultimo indice del arreglo
 * @param k Indice del elemento buscado
 * @param comp_f Funcion de comparacion (-1 si a < b, 1 si a > b, 0 si a = b)
 * @return Player* Puntero al k-ésimo elemento de V
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