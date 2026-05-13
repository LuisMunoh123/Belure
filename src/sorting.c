/**
 * @file sorting.c
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Sorting functions.
 */

#include"sorting.h"
#include"player.h"

/**
 * @brief Internal swap function.
 * 
 * @param a Pointer to the element to swap with b.
 * @param b Pointer to the element to swap with a.
 */
static void swap(Player *a, Player *b) {
	Player temp = *a;
	*a = *b;
	*b = temp;
}

/**
 * @brief Swap sort.
 * 
 * @param V Array of players.
 * @param n Array size.
 * @param comp_f Comparison function (-1 if a < b, 1 if a > b, 0 if a = b).
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
 * @brief Insertion sort.
 * 
 * @param V Array of players.
 * @param n Array size.
 * @param comp_f Comparison function (-1 if a < b, 1 if a > b, 0 si a = b).
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
 * @brief Selection sort.
 * 
 * @param V Array of players.
 * @param n Array size.
 * @param comp_f Comparison function (-1 if a < b, 1 if a > b, 0 if a = b).
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
 * @brief Cocktail shaker sort.
 * 
 * @param V Array of players.
 * @param n Array size.
 * @param comp_f Comparison function (-1 if a < b, 1 if a > b, 0 if a = b).
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
 * @brief Quicksort.
 * 
 * @param V Array of players.
 * @param left Sort start index.
 * @param right Sort end index.
 * @param comp_f Comparison function (-1 if a < b, 1 if a > b, 0 if a = b).
 */
void quick_sort(Player V[], int left, int right, int (*comp_f)(Player *, Player *))
{
	// Arreglos de 1 elemento no se ordenan
	if ((right - left)+1 < 2)
		return;
	
	// Eleccion del pivote por mediana de 3
	int pivot_pos = median_of_three(V, left, right, comp_f);

	pivot_pos = lomuto_partition(V, left, right, pivot_pos, comp_f);

	// llamadas recursivas
	quick_sort(V, left, pivot_pos - 1, comp_f);
	quick_sort(V, pivot_pos + 1, right, comp_f);
}

/**
 * @brief Merges two already sorted subarrays.
 *
 * @param V Array of players.
 * @param left Start of the array.
 * @param mid Middle of the array.
 * @param right End of the array.
 * @param comp_f Comparison function.
 */
static void merge(Player V[], int left, int mid, int right, int (*comp_f)(Player *, Player *)) {
	int i = left;
	int j = mid + 1;
	int k = 0;
	Player temp[right - left + 1];

	while (i <= mid && j <= right) {
		if (comp_f(&V[i], &V[j]) <= 0) {
			temp[k++] = V[i++];
		} else {
			temp[k++] = V[j++];
		}
	}

	while (i <= mid) {
		temp[k++] = V[i++];
	}

	while (j <= right) {
		temp[k++] = V[j++];
	}

	for (i = left, k = 0; i <= right; i++, k++) {
		V[i] = temp[k];
	}
}

/**
 * @brief Classic merge sort.
 *
 * @param V Array of players.
 * @param left Start of the array.
 * @param right End of the array.
 * @param comp_f Comparison function.
 */
void merge_sort_classic(Player V[], int left, int right, int (*comp_f)(Player *, Player *)) {
	if (left < right) {
		int mid = left + (right - left) / 2;

		merge_sort_classic(V, left, mid, comp_f);
		merge_sort_classic(V, mid + 1, right, comp_f);
		merge(V, left, mid, right, comp_f);
	}
}

/**
 * @brief Optimized merge sort: uses insertion sort when the subarray
 *        is smaller than or equal to the given threshold.
 *
 * @param V Array of players.
 * @param left Start of the array.
 * @param right End of the array.
 * @param threshold Minimum size to use insertion sort.
 * @param comp_f Comparison function.
 */
void merge_sort_optimized(Player V[], int left, int right, int threshold, int (*comp_f)(Player *, Player *)) {
	if (right - left + 1 <= threshold) {
		insertion_sort(&V[left], right - left + 1, comp_f);
		return;
	}
	if (left < right) {
		int mid = left + (right - left) / 2;
		merge_sort_optimized(V, left, mid, threshold, comp_f);
		merge_sort_optimized(V, mid + 1, right, threshold, comp_f);
		merge(V, left, mid, right, comp_f);
	}
}

/**
 * @brief Partitions an array using the Lomuto algorithm.
 * 
 * @param V Array of players.
 * @param left Partition start.
 * @param right Partition end.
 * @param pivot Pivot position.
 * @param comp_f Comparison function (-1 if a < b, 1 if a > b, 0 if a = b).
 * @return int Final pivot position.
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
