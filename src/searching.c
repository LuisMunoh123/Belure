/**
 * @file searching.c
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
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
 * @param comp_f funcion de comparacion (-1 si a < b, 1 si a > b, 0 si a = b)
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

/**
 * @brief Funcion de busqueda binaria iterativa
 * * @param V Arreglo de jugadores
 * @param beg Indice inicial
 * @param end Indice final
 * @param x Puntero al jugador a buscar (con el criterio clave lleno)
 * @param comp_f Puntero a la función de comparación (ej: compare_id)
 * @return int m Indice de la busqueda, retorna -1 si no se encuentra
 */
int binary_search_iterative(Player V[], int beg, int end, Player *x, int (*comp_f)(Player *, Player *)) {
	while (beg <= end) {
		int mid = beg + (end - beg) / 2;
		int cmp = comp_f(&V[mid], x);

		if (cmp == 0) {
			return mid;
		} else if ( cmp < 0) {
			beg = mid + 1;
		} else {
			end = mid - 1;
		}
	}
	return -1;
}
/**
 * @brief Funcion de busqueda binaria recursiva
 * * @param V Arreglo de jugadores
 * @param beg Indice inicial
 * @param end Indice final
 * @param x Puntero al jugador a buscar (con el criterio clave lleno)
 * @param comp_f Puntero a la función de comparación (ej: compare_id)
 * @return int Indice donde se encuentra el elemento, retorna -1 si no se encuentra
 */
int binary_search_recursive(Player V[], int beg, int end, Player *x,int (*comp_f)(Player *, Player *)) {
	if (beg > end) {
		return -1;
	}

	int mid = beg + (end - beg) / 2;
	int cmp = comp_f(&V[mid], x);

	if (cmp == 0) {
		return mid;
	}

	if (cmp > 0) {
		return binary_search_recursive(V, beg, mid - 1, x, comp_f);
	}
	return binary_search_recursive(V, mid + 1, end, x, comp_f);
}

/**
 * @brief Funcion de busqueda binaria que busca la primera aparicion de un elemento en un arreglo ordenado
 * * @param V Arreglo de jugadores
 * @param beg Indice inicial
 * @param end Indice final
 * @param x Elemento a buscar
 * @param comp_f Puntero a la función de comparación (ej: compare_id)
 * @return int Indice donde se encuentra la primera aparicion, retorna -1 si no se encuentra
 */
int binary_search_first(Player V[], int beg, int end, Player *x, int (*comp_f)(Player *, Player *)) {
	int result = -1;

	while (beg <= end) {
		int mid = beg + (end - beg) / 2;
		int cmp = comp_f(&V[mid], x);

		if (cmp == 0) {
			result = mid;
			end = mid - 1;
		} else if (cmp < 0) {
			beg = mid + 1;
		} else {
			end = mid - 1;
		}
	}
	return result;
}

/**
 * @brief Funcion de busqueda binaria que busca la ultima aparicion de un elemento en un arreglo ordenado
 * * @param V Arreglo de jugadores
 * @param beg Indice inicial
 * @param end Indice final
 * @param x Elemento a buscar
 * @param comp_f Puntero a la función de comparación (ej: compare_id)
 * @return int Indice donde se encuentra la ultima aparicion, retorna -1 si no se encuentra
 */
int binary_search_last(Player V[], int beg, int end, Player *x, int (*comp_f)(Player *, Player *)) {
	int result = -1;

	while (beg <= end) {
		int mid = beg + (end - beg) / 2;
		int cmp = comp_f(&V[mid], x);

		if (cmp == 0) {
			result = mid;
			beg = mid + 1;
		} else if (cmp < 0) {
			beg = mid + 1;
		} else {
			end = mid - 1;
		}
	}
	return result;
}

/**
 * @brief Funcion de busqueda binaria que busca el rango completo de un elemento en un arreglo ordenado
 * @param V Arreglo de jugadores
 * @param beg Indice inicial
 * @param end Indice final
 * @param x Elemento a buscar
 * @param comp_f Puntero a la función de comparación (ej: compare_id)
 * @param first puntero donde guardar la primera posicion
 * @param last puntero donde guardar la ultima aparicion
 */
void binary_search_range(Player V[], int beg, int end, Player *x, int (*comp_f)(Player *, Player *), int *first, int *last) {
	*first = binary_search_first(V, beg, end, x, comp_f);

	if (*first == -1) {
		*last = -1;
		return;
	}

	*last = binary_search_last(V, *first, end, x, comp_f);
}

/**
 * @brief Busca la primera posicion cuyo valor sea mayor o igual al buscado
 * 
 * @param V Arreglo de jugadores ordenado
 * @param beg Indice inicial
 * @param end Indice final
 * @param x Elemento limite inferior
 * @param comp_f Funcion de comparacion
 * @return int Primera posicion valida, o -1 si no existe
 */
int binary_search_lower_bound(Player V[], int beg, int end, Player *x, int (*comp_f)(Player *, Player *)) {
	int result = -1;

	while (beg <= end) {
		int mid = beg + (end - beg) / 2;
		int cmp = comp_f(&V[mid], x);

		if (cmp >= 0) {
			result = mid;
			end = mid - 1;
		} else {
			beg = mid + 1;
		}
	}

	return result;
}

/**
 * @brief Busca la ultima posicion cuyo valor sea menor o igual al buscado
 * 
 * @param V Arreglo de jugadores ordenado
 * @param beg Indice inicial
 * @param end Indice final
 * @param x Elemento limite superior
 * @param comp_f Funcion de comparacion
 * @return int Ultima posicion valida, o -1 si no existe
 */
int binary_search_upper_bound(Player V[], int beg, int end, Player *x, int (*comp_f)(Player *, Player *)) {
	int result = -1;

	while (beg <= end) {
		int mid = beg + (end - beg) / 2;
		int cmp = comp_f(&V[mid], x);

		if (cmp <= 0) {
			result = mid;
			beg = mid + 1;
		} else {
			end = mid - 1;
		}
	}

	return result;
}

/**
 * @brief Funcion de busqueda exponencial
 * 
 * @param V Arreglo de jugadores
 * @param n cantidad o tamanho del arreglo
 * @param x elemento a buscar
 * @param comp_f funcion de comparacion (-1 si a < b, 1 si a > b, 0 si a = b)
 * @return int Indice donde se encuentra el elemento, o -1 si no se encuentra
 */
int exponential_search(Player V[], int n, Player *x, int (*comp_f)(Player *, Player *)) {
	if (n <= 0) {
		return -1;
	}

	if (comp_f(&V[0], x) == 0) {
		return 0;
	}

	int bound = 1;

	while (bound < n && comp_f(&V[bound], x) < 0) {
		bound *= 2;
	}

	int beg = bound / 2;
	int end;

	if (bound < n) {
		end = bound;
	} else {
		end = n - 1;
	}

	return binary_search_iterative(V, beg, end, x, comp_f);
}

/**
 * @brief Funcion de busqueda por interpolacion
 * 
 * @param V Arreglo de jugadores
 * @param beg Indice inicial
 * @param end Indice final
 * @param target_id Id a buscar
 * @return int Indice donde se encuentra el jugador, o -1 si no se encuentra
 */
int interpolation_search(Player V[], int beg, int end, int target_id) {
	while (beg <= end && target_id >= V[beg].id && target_id <= V[end].id) {
		if (V[beg].id == V[end].id) {
			if (V[beg].id == target_id) {
				return beg;
			}
			return -1;
		}

		long long numerator = (long long)(target_id - V[beg].id) * (end - beg);
		long long denominator = (long long)(V[end].id - V[beg].id);
		int pos = beg + (int)(numerator / denominator);

		if (pos < beg) {
			pos = beg;
		} else if (pos > end) {
			pos = end;
		}

		if (V[pos].id == target_id) {
			return pos;
		}

		if (V[pos].id < target_id) {
			beg = pos + 1;
		} else {
			end = pos - 1;
		}
	}
	return -1;
}
