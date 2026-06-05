/**
 * @file player.c
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Implementation of the comparison functions for the Player structure.
 */

#include "player.h"
#include "utilities.h"

/**
 * @brief Swaps two players.
 * @param p1 Pointer to player 1.
 * @param p2 Pointer to player 2.
 */
void swap_player(Player *p1, Player *p2) {
	Player temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}

/**
 * @brief Compares whether two players are exactly equal in all their fields.
 * * @param p1 Pointer to the first player.
 * @param p2 Pointer to the second player.
 * @return true If all fields are equivalent.
 * @return false If at least one field differs.
 */
bool are_players_equal(Player *p1, Player *p2) {
	// Validar que los punteros no sean nulos
	if (p1 == NULL || p2 == NULL) return false;

	// Comparar campos numericos y booleanos
	if (p1->id != p2->id) return false;
	if (p1->score != p2->score) return false;
	if (p1->competitions != p2->competitions) return false;

	// Comparar arreglos de caracteres estaticos (name y team)
	if (strcasecmp(p1->name, p2->name) != 0) return false;
	if (strcasecmp(p1->team, p2->team) != 0) return false;

	return true;
}

/**
 * @brief Compares two players based on their ID.
 * * Performs a direct numeric comparison of the `id` field.
 * * @param p1 Pointer to the first player.
 * @param p2 Pointer to the second player.
 * @return -1 if p1's ID is smaller, 1 if it is larger, 0 if they are equal.
 */
int compare_id(Player *p1, Player *p2) {
	return p1->id - p2->id;
}

/**
 * @brief Compares two players alphabetically by name.
 * * Uses the standard strcmp function to determine lexicographic order.
 * * @param p1 Pointer to the first player.
 * @param p2 Pointer to the second player.
 * @return -1 if p1's name comes first, 1 if it comes after, 0 if they are equal.
 */
int compare_name(Player *p1, Player *p2) {
	return strcasecmp(p1->name, p2->name);
}

/**
 * @brief Compares two players alphabetically by their team name.
 * * Uses the standard strcmp function to evaluate the team string.
 * * @param p1 Pointer to the first player.
 * @param p2 Pointer to the second player.
 * @return -1 if p1's team comes first, 1 if it comes after, 0 if they are equal.
 */
int compare_team(Player *p1, Player *p2) {
	return strcasecmp(p1->team, p2->team);
}

/**
 * @brief Compares two players based on their score.
 * * Evaluates the floating-point values of the `score` field.
 * * @param p1 Pointer to the first player.
 * @param p2 Pointer to the second player.
 * @return -1 if p1's score is lower, 1 if it is higher, 0 if they are equal.
 */
int compare_score(Player *p1, Player *p2) {
	if (p1->score > p2->score) return 1;
	if (p1->score < p2->score) return -1;
	return 0;
}
/**
 * @brief Compares two players based on the number of competitions.
 * * Compares the integer values of the `competitions` field for each player.
 * * @param p1 Pointer to the first player.
 * @param p2 Pointer to the second player.
 * @return -1 if p1's competitions are fewer, 1 if they are greater, 0 if they are equal.
 */
int compare_competition(Player *p1, Player *p2) {
	return p1->competitions - p2->competitions;
}


/**
 * @brief Prints a player to the console.
 * 
 * @param player Pointer to the player to print.
 */
void print_player(Player *player) {
	printf(EVEN_DARKER_GREEN "╔════════════════════╗\n");
	printf(EVEN_DARKER_GREEN "║ " LIGHT_GRAY"ID: "DARK_GRAY"%4d" EVEN_DARKER_GREEN "           ║\n", player->id);
	printf(EVEN_DARKER_GREEN "║ " DARK_YELLOW"Name: "YELLOW"%10s" EVEN_DARKER_GREEN "   ║\n", player->name);
	printf(EVEN_DARKER_GREEN "║ " DARK_BLUE"Team: "LIGHT_BLUE"%7s" EVEN_DARKER_GREEN "      ║\n", player->team);
	printf(EVEN_DARKER_GREEN "║ " DARK_GREEN"Score: "LIGHT_GREEN"%.1f" EVEN_DARKER_GREEN "         ║\n", player->score);
	printf(EVEN_DARKER_GREEN "║ " PURPLE"Competitions: "MAGENTA"%4d" EVEN_DARKER_GREEN " ║\n", player->competitions);
	printf(EVEN_DARKER_GREEN "╚════════════════════╝\n");
	printf(RESET"\n");
}

/**
 * @brief Prints the player array in table format.
 * * @param players Player array.
 * @param n Array size.
 */
void print_player_array(Player *players, int n)
{
	if (n > MAX_CONSOLE_READABLE_PLAYERS) {
		n = MAX_CONSOLE_READABLE_PLAYERS;
		print_error(301, NULL, NULL);
	}
	// Imprimimos cabecera
	printf(
		DARK_GRAY "|" RESET " "
		LIGHT_GRAY "%4s" RESET " "
		DARK_GRAY "|" RESET " "
		DARK_YELLOW "%10s" RESET " "
		DARK_GRAY "|" RESET " "
		DARK_BLUE "%7s" RESET " "
		DARK_GRAY "|" RESET " "
		DARK_GREEN "%4s" RESET " "
		DARK_GRAY "|" RESET " "
		PURPLE "%3s" RESET " "
		DARK_GRAY "|" RESET "\n",
		"ID", "NAME", "TEAM", "SCORE", "COMPS"
	);

	// Imprimimos los datos
	for (int i = 0; i < n; i++) {
		printf(
			DARK_GRAY "|" RESET " "
			WHITE "%4d" RESET " "
			DARK_GRAY "|" RESET " "
			YELLOW "%10s" RESET " "
			DARK_GRAY "|" RESET " "
			LIGHT_BLUE "%7s" RESET " "
			DARK_GRAY "|" RESET " "
			LIGHT_GREEN "%5.1f" RESET " "
			DARK_GRAY "|" RESET " "
			MAGENTA "%5d" RESET " "
			DARK_GRAY "|" RESET "\n",
			players[i].id,
			players[i].name,
			players[i].team,
			players[i].score,
			players[i].competitions
		);
	}
	printf(RESET"\n");
}

/**
 * @brief Prints a player array in page format; used together with print_player_array_more.
 * 
 * @param players Players to print.
 * @param start Starting index of the page to show (inclusive).
 * @param end Ending index of the page to show (exclusive).
 */
static void print_player_array_page(Player *players, int start, int end)
{
	printf(
		DARK_GRAY "|" RESET " "
		LIGHT_GRAY "%4s" RESET " "
		DARK_GRAY "|" RESET " "
		DARK_YELLOW "%10s" RESET " "
		DARK_GRAY "|" RESET " "
		DARK_BLUE "%7s" RESET " "
		DARK_GRAY "|" RESET " "
		DARK_GREEN "%4s" RESET " "
		DARK_GRAY "|" RESET " "
		PURPLE "%3s" RESET " "
		DARK_GRAY "|" RESET "\n",
		"ID", "NAME", "TEAM", "SCORE", "COMPS"
	);

	for (int i = start; i < end; i++) {
		printf(
			DARK_GRAY "|" RESET " "
			WHITE "%4d" RESET " "
			DARK_GRAY "|" RESET " "
			YELLOW "%10s" RESET " "
			DARK_GRAY "|" RESET " "
			LIGHT_BLUE "%7s" RESET " "
			DARK_GRAY "|" RESET " "
			LIGHT_GREEN "%5.1f" RESET " "
			DARK_GRAY "|" RESET " "
			MAGENTA "%5d" RESET " "
			DARK_GRAY "|" RESET "\n",
			players[i].id,
			players[i].name,
			players[i].team,
			players[i].score,
			players[i].competitions
		);
	}
}

/**
 * @brief Clears the current page from print_player_array_page().
 * 
 */
static void clear_player_page()
{
	for (int i = 0; i < PAGE_LINES; i++) {
		printf("\r\033[2K"); // Mueve el cursor al inicio de la linea y limpia la linea
		if (i < PAGE_LINES - 1) {
			printf("\033[1A"); // Mueve el cursor hacia arriba una linea
		}
	}
	printf("\r"); // Mueve el cursor al inicio de la linea
	fflush(stdout);
	printf(RESET);
}

/**
 * @brief Shows players in a UNIX 'more' style view.
 * 
 * @param players Players to print.
 * @param n Number of players.
 */
void print_player_array_more(Player *players, int n)
{
	int start = 0;

	//getchar(); // Antes era en un parche, ahora se quito para el funcionamiento de getopt
	while (start < n) {
		int end = start + PAGE_SIZE;
		if (end > n) {
			end = n;
		}

		print_player_array_page(players, start, end);

		if (end == n) {
			printf(RESET "\n");
			return;
		}

		printf("\n" DARK_GRAY "--More--       |" LIGHT_GRAY " Enter: next " DARK_GRAY "|" LIGHT_GRAY " q: quit " DARK_GRAY "| ["LIGHT_GRAY"%d"DARK_GRAY"-"LIGHT_GRAY"%d" DARK_GRAY" of " LIGHT_GRAY "%d" DARK_GRAY "] Action:  " LIGHT_GRAY, start + 1, end, n);
		fflush(stdout);

		int c = getchar();
		
		if (c == 'q' || c == 'Q') {
			while (c != '\n' && c != EOF) {
				c = getchar();
			}
			printf("\n");
			return;
		}

		while (c != '\n' && c != EOF) {
			c = getchar();
		}

		printf("\033[1A"); // Mueve el cursor hacia arriba una linea
		clear_player_page();

		start = end;
	}
}

/**
 * @brief Computes the median of three for an array of players.
 * 
 * @param V Array of players.
 * @param left Starting index of the array.
 * @param right Ending index of the array.
 * @param comp_f Comparison function (-1 if a < b, 1 if a > b, 0 if a = b).
 * @return int Index of the median of three.
 */
int median_of_three(Player V[], int left, int right, int (*comp_f)(Player *, Player *))
{
	int mid = (left + right) / 2;
	if (comp_f(&V[left], &V[mid]) > 0) { // a > b
		if (comp_f(&V[mid], &V[right]) > 0) { // a > b > c, mediana es b
			return mid;
		} else if (comp_f(&V[left], &V[right]) > 0) { // a > c >= b, mediana es c
			return right;
		} else { // c >= a > b, mediana es a
			return left;
		}
	} else { // a <= b
		if (comp_f(&V[left], &V[right]) > 0) { // b >= a > c, mediana es a
			return left;
		} else if (comp_f(&V[mid], &V[right]) > 0) { // b > c >= a, mediana es c
			return right;
		} else { // c >= b >= a, mediana es b
			return mid;
		}
	}
}