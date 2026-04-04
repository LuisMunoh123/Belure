/**
 * @file searching.c
 * @author Andres Barbosa, Milton Hernández, Ivan Gallardo
 * @brief Funciones de busqueda
 */

#include"searching.h"

/**
 * @brief Funcion de busqueda lineal
 * 
 * @param V Arreglo de enteros
 * @param n Tamanho del arreglo
 * @param x Busqueda
 * @return int r Indice de la busqueda, retorna n si no se encuentra
 */
int linear_search(int V[], int n, int x) {
    int r = n;

    for (int i = 0; i < n; i++) {
        if (V[i] == x) {
            r = i;
            break;
        }
    }

    return r;
}

/**
 * @brief Funcion de busqueda binaria
 * 
 * @param V Arreglo de enteros
 * @param beg Indice inicial
 * @param end Indice final
 * @param x Busqueda
 * @return int m Indice de la busqueda, retorna -1 si no se encuentra
 */
int binary_search(int V[], int beg, int end, int x) {
    while (beg <= end) {
        int m = beg + (end - beg) / 2;

        if (V[m] == x) {
            return m;
        } else if (V[m] < x) {
            beg = m + 1;
        } else {
            end = m - 1;
        }
    }

    return -1;
}