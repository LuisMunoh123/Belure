/**
 * @file sorting.c
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Funciones de ordenamiento
 */

#include"sorting.h"
#include"player.h"

/**
 * @brief Funcion interna de intercambio
 * 
 * @param a puntero a elemento a intercambiar por b
 * @param b puntero a elemento a intercambiar por a
 */
static void swap(Player *a, Player *b) {
	Player temp = *a;
	*a = *b;
	*b = temp;
}

/**
 * @brief Ordenamiento por intercambio
 * 
 * @param V arreglo de enteros
 * @param n tamanho del arreglo
 * @param comp_f funcion de comparacion (-1 si a < b, 1 si a > b, 0 si a = b)
 */
void swap_sort(Player V[], int n,int(*comp_f)( Player *, Player *)) {
	for (int i = 1; i <= n - 1; i++) {
		int swapped = 0;

		for (int j = 0; j <= n - 1 - i; j++) {
			if (comp_f(&V[j], &V[j + 1]) > 0) {
				swap(&V[j], &V[j + 1]);
				swapped = 1;
			}
		}

		if (swapped == 0) {
			break;
		}
	}
}

/**
 * @brief Ordenamiento por insercion
 * 
 * @param V arreglo de enteros
 * @param n tamanho del arreglo
 * @param comp_f funcion de comparacion (-1 si a < b, 1 si a > b, 0 si a = b)
 */
void insertion_sort(Player V[], int n,int(*comp_f)( Player *, Player *)) {
	for (int i = 1; i <= n - 1; i++) {
		Player k = V[i];
		int j = i - 1;

		while ((j >= 0) && (comp_f(&V[j], &k) > 0)) {
			V[j + 1] = V[j];
			j--;
		}

		V[j + 1] = k;
	}
}

/**
 * @brief Ordenamiento por seleccion
 * 
 * @param V arreglo de enteros
 * @param n tamanho del arreglo
 * @param comp_f funcion de comparacion (-1 si a < b, 1 si a > b, 0 si a = b)
 */
void selection_sort(Player V[], int n, int (*comp_f)(Player *, Player *)) {
	for (int i = 1; i <= n - 1; i++) {
		int k = i - 1;
		for (int j = i; j <= n - 1; j++) {
			if (comp_f(&V[j], &V[k]) < 0) {
				k = j;
			}
		}
		if (k != i - 1) {
			swap(&V[k], &V[i - 1]);
		}
	}
}

/**
 * @brief Ordenamiento por shaker
 * 
 * @param V Arreglo de jugadores
 * @param n Tamanho del arreglo
 * @param comp_f Funcion de comparacion (-1 si a < b, 1 si a > b, 0 si a = b)
 */
void cocktail_shaker_sort(Player V[], int n, int (*comp_f)(Player *, Player *)) {
	int beg = 0;
	int end = n - 1;
	int swapped;

	while (beg < end) {
		swapped = 0;
		for (int i = beg; i <= end - 1; i++) {
			if (comp_f(&V[i], &V[i + 1]) > 0) {
				swap(&V[i], &V[i + 1]);
				swapped = 1;
			}
		}
		for (int i = end - 1; i >= beg + 1; i--) {
			if (comp_f(&V[i], &V[i - 1]) < 0) {
				swap(&V[i], &V[i - 1]);
				swapped = 1;
			}
		}
		if (swapped == 0) break;
		beg++;
		end--;
	}
}

/**
 * @brief Ordenamiento por quicksort
 * 
 * @param V Arreglo de Players
 * @param left Inicio del ordenamiento
 * @param right Final del ordenamiento
 * @param comp_f Funcion de comparacion (-1 si a < b, 1 si a > b, 0 si a = b)
 * @param pivot_type Tipo de pivote (1 = menor, 2 = mayor, 3 = mediana de 3, 4 = aleatorio)
 */
void quick_sort(Player V[], int left, int right, int (*comp_f)(Player *, Player *), int pivot_type)
{
	// Arreglos de 1 elemento no se ordenan
	if ((right - left)+1 < 2)
		return;
	
	// Eleccion del pivote 1=menor, 2=mayor, 3=mediana de 3, 4=aleatorio
	int pivot_pos = 0;
	switch (pivot_type) {
		case 1:
			pivot_pos = left;
			break;
		case 2:
			pivot_pos = right;
			break;
		case 3:
			pivot_pos = median_of_three(V, left, right, comp_f);
			break;
		case 4:
			pivot_pos = rand() % (right - left + 1) + left;
			break;
	}

	pivot_pos = lomuto_partition(V, left, right, pivot_pos, comp_f);

	// llamadas recursivas
	quick_sort(V, left, pivot_pos - 1, comp_f, pivot_type);
	quick_sort(V, pivot_pos + 1, right, comp_f, pivot_type);
}

/**
 * @brief Particion de un arreglo por algoritmo lomuto
 * 
 * @param V Arreglo de Players
 * @param left Inicio de la particion
 * @param right Fin de la particion
 * @param pivot Posicion del pivote
 * @param comp_f Funcion de comparacion (-1 si a < b, 1 si a > b, 0 si a = b)
 * @return int Posicion final del pivote
 */
int lomuto_partition(Player V[], int left, int right, int pivot, int (*comp_f)(Player *, Player *))
{
	swap_player(&V[pivot], &V[right]);
	int l = left-1;
	for (int i = left; i <= right-1; i++) {
		if (comp_f(&V[i], &V[right]) <= 0) {
			l++;
			swap_player(&V[l], &V[i]);
		}
	}
	// Ubicamos el pivote en la posicion correcta
	swap_player(&V[l+1], &V[right]);
	return l+1;
}