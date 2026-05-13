/**
 * @file searching.c
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Search functions.
 */

#include"searching.h"
#include"player.h"

/**
 * @brief Linear search function.
 * 
 * @param V Array of players.
 * @param n Array size.
 * @param x Target player.
 * @param comp_f Comparison function (-1 if a < b, 1 if a > b, 0 if a = b).
 * @return int r Search index, returns n if not found.
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
 * @brief Iterative binary search function.
 * * @param V Array of players.
 * @param beg Starting index.
 * @param end Ending index.
 * @param x Pointer to the player to search for (with the key criterion filled in).
 * @param comp_f Pointer to the comparison function (e.g. compare_id).
 * @return int m Search index, returns -1 if not found.
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
 * @brief Recursive binary search function.
 * * @param V Array of players.
 * @param beg Starting index.
 * @param end Ending index.
 * @param x Pointer to the player to search for (with the key criterion filled in).
 * @param comp_f Pointer to the comparison function (e.g. compare_id).
 * @return int Index where the element is found, returns -1 if not found.
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
 * @brief Binary search function that finds the first occurrence of an element in a sorted array.
 * * @param V Array of players.
 * @param beg Starting index.
 * @param end Ending index.
 * @param x Element to search for.
 * @param comp_f Pointer to the comparison function (e.g. compare_id).
 * @return int Index where the first occurrence is found, returns -1 if not found.
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
 * @brief Binary search function that finds the last occurrence of an element in a sorted array.
 * * @param V Array of players.
 * @param beg Starting index.
 * @param end Ending index.
 * @param x Element to search for.
 * @param comp_f Pointer to the comparison function (e.g. compare_id).
 * @return int Index where the last occurrence is found, returns -1 if not found.
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
 * @brief Binary search function that finds the full range of an element in a sorted array.
 * @param V Array of players.
 * @param beg Starting index.
 * @param end Ending index.
 * @param x Element to search for.
 * @param comp_f Pointer to the comparison function (e.g. compare_id).
 * @param first Pointer where the first position is stored.
 * @param last Pointer where the last occurrence is stored.
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
 * @brief Finds the first position whose value is greater than or equal to the target.
 * 
 * @param V Sorted player array.
 * @param beg Starting index.
 * @param end Ending index.
 * @param x Lower-bound element.
 * @param comp_f Comparison function.
 * @return int First valid position, or -1 if none exists.
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
 * @brief Finds the last position whose value is less than or equal to the target.
 * 
 * @param V Sorted player array.
 * @param beg Starting index.
 * @param end Ending index.
 * @param x Upper-bound element.
 * @param comp_f Comparison function.
 * @return int Last valid position, or -1 if none exists.
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
 * @brief Exponential search function.
 * 
 * @param V Array of players.
 * @param n Number or size of the array.
 * @param x Element to search for.
 * @param comp_f Comparison function (-1 if a < b, 1 if a > b, 0 if a = b).
 * @return int Index where the element is found, or -1 if not found.
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
 * @brief Interpolation search function.
 * 
 * @param V Array of players.
 * @param beg Starting index.
 * @param end Ending index.
 * @param target_id ID to search for.
 * @return int Index where the player is found, or -1 if not found.
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
