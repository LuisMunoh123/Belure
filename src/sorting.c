/**
 * @file sorting.c
 * @author Andres Barbosa, Milton Hernández, Ivan Gallardo
 * @brief Funciones de ordenamiento
 */

#include"sorting.h"

/**
 * @brief Funcion interna de intercambio
 * 
 * @param a puntero a elemento a intercambiar por b
 * @param b puntero a elemento a intercambiar por a
 */
static void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

/**
 * @brief Ordenamiento por intercambio
 * 
 * @param V arreglo de enteros
 * @param n tamanho del arreglo
 */
void swap_sort(int V[], int n) {
	for (int i = 1; i <= n - 1; i++) {
		int swapped = 0;

		for (int j = 0; j <= n - 1 - i; j++) {
			if (V[j] > V[j + 1]) {
				swap(&V[j], &V[j + 1]);
				swapped = 1;
			}
		}

		if (swapped == 0) {
			printf("There was no swapping\n");
			break;
		}
	}
}

/**
 * @brief Ordenamiento por insercion
 * 
 * @param V arreglo de enteros
 * @param n tamanho del arreglo
 */
void insertion_sort(int V[], int n) {
    for (int i = 1; i <= n - 1; i++) {
        int k = V[i];
        int j = i - 1;

        while ((j >= 0) && (V[j] > k)) {
            swap(&V[j + 1], &V[j]);
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
 */
void selection_sort(int V[], int n) {
    for (int i = 1; i <= n - 1; i++) {
        int k = i - 1;

        for (int j = i; j <= n - 1; j++) {
            if (V[j] < V[k]) {
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
 * @param V arreglo de enteros
 * @param n tamanho del arreglo
 */
void cocktail_shaker_sort(int V[], int n) {
    int beg = 0;
    int end = n - 1;
    int swapped;

    while (beg < end) {
        swapped = 0;

        for (int i = beg; i <= end - 1; i++) {
            if (V[i] > V[i + 1]) {
                swap(&V[i], &V[i + 1]);
                swapped = 1;
            }
        }

        for (int i = end - 1; i >= beg + 1; i--) {
            if (V[i] < V[i - 1]) {
                swap(&V[i], &V[i - 1]);
                swapped = 1;
            }
        }

        if (swapped == 0) {
            printf("In the last iteration there was no swapping\n");
			break;
        }

        beg++;
        end--;
    }
}
