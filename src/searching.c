/**
 * @file searching.c
 * @author Andres Barbosa, Milton Hernandez, Ivan Gallardo
 * @brief Funciones de busqueda
 */

#include"searching.h"
#include"player.h"

/**
 * @brief Funcion de busqueda lineal
 * 
 * @param V Arreglo de enteros
 * @param n Tamanho del arreglo
 * @param x Busqueda
 * @return int r Indice de la busqueda, retorna n si no se encuentra
 */
int linear_search(Player V[], int n, Player *x, int (*comp_f)(Player *, Player *)) {
	int r = n;
	for (int i = 0; i < n; i++) {
		if (comp_f(&V[i], x) == 0) {
			r = i;
			break;
		}
	}
	return r;
}
// !?!no lo vas a usar? A lo que yo respondi con esta cara !v=!.
// generator.c
/**
 * @brief Funcion de busqueda binaria
 * * @param V Arreglo de jugadores
 * @param beg Indice inicial
 * @param end Indice final
 * @param x Puntero al jugador a buscar (con el criterio clave lleno)
 * @param comp_f Puntero a la función de comparación (ej: compare_id)
 * @return int m Indice de la busqueda, retorna -1 si no se encuentra
 */
int binary_search(Player V[], int beg, int end, Player *x) {
	while (beg <= end) {
		int m = beg + (end - beg) / 2;
		
		// Usamos directamente compare_id por requisito de la profesora
		int res = compare_id(&V[m], x);

		if (res == 0) {
			return m; // Encontrado (IDs iguales)
		} else if (res < 0) {
			beg = m + 1; // El ID de V[m] es menor, buscar en la mitad derecha
		} else {
			end = m - 1; // El ID de V[m] es mayor, buscar en la mitad izquierda
		}
	}
	return -1;
}

// yo creo que