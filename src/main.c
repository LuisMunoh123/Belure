/**
 * @file main.c
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Funciones principales del programa
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>

#include "generator.h"
#include "sorting.h"
#include "searching.h"
#include "utilities.h"
#include "player.h" 
#include "generate_exec_times.h"

static struct option long_options[] = {
	{"generate", required_argument, 0, 'g'},
	{"read", no_argument, 0, 'r'},
	{"sort", no_argument, 0, 's'},
	{"find", no_argument, 0, 'f'},
	{"experiment", no_argument, 0, 'e'},
	{"exit", no_argument, 0, 'x'},
	{"type", required_argument, 0, 't'},
	{"algorithm", required_argument, 0, 'a'},
	{"criteria", required_argument, 0, 'c'},
	{"binary", required_argument, 0, 'b'},
	{"id", required_argument, 0, 'i'},
	{"help", no_argument, 0, 'h'},
	{0, 0, 0, 0}
};

static void print_usage(const char *progname);

int main(int argc, char *argv[])
{
	int opt;
	char action = '\0';
	int action_count = 0;

	int generationType = 0;
	Player *players = NULL;
	int n = -1;

	int sortOption = 0;
	int sortCriteria = 0;

	int searchOption = 0;
	int searchId = -1;
	int result;

	srand(time(0));

	while ((opt = getopt_long(argc, argv, "g:rsfxeht:a:c:b:i:", long_options, NULL)) != -1) {
		switch (opt) {
			case 'g':
				action = 'g';
				action_count++;
				n = atoi(optarg);
				break;

			case 'r':
			case 's':
			case 'f':
			case 'e':
			case 'x':
				action = (char)opt;
				action_count++;
				break;

			case 't':
				generationType = atoi(optarg);
				break;

			case 'a':
				sortOption = atoi(optarg);
				break;

			case 'c':
				sortCriteria = atoi(optarg);
				break;

			case 'b':
				searchOption = atoi(optarg);
				break;

			case 'i':
				searchId = atoi(optarg);
				break;

			case 'h':
				print_usage(argv[0]);
				return 0;

			default:
				print_usage(argv[0]);
				return 1;
		}
	}

	if (action_count != 1) {
		printf("Error: debes indicar exactamente una accion.\n\n");
		print_usage(argv[0]);
		return 1;
	}

	if (action == 'g') {
		if (n < 0 || generationType < 1 || generationType > 3) {
			printf("Error: para generar debes usar -n <cantidad> y -t <1|2|3>\n\n");
			print_usage(argv[0]);
			return 1;
		}

		if (generationType == 1) {
			printf(YELLOW "\nGenerating SORTED array...\n");
		} else if (generationType == 2) {
			printf(YELLOW "\nGenerating INVERSE array...\n");
		} else {
			printf(YELLOW "\nGenerating SHUFFLED array...\n");
		}

		return generate_csv(n, generationType);
	}

	if (action == 'r') {
		if ((players = load_players("build/db/players.csv", &n)) == NULL) {
			print_error(101, "build/db/players.csv", NULL);
			return 1;
		}

		printf(ORANGE "\nplayers.csv:\n" RESET);
		print_player_array_more(players, n);
		free(players);
		return 0;
	}

	if (action == 's') {
		int (*comp_ptr)(Player*, Player*) = NULL;

		if (sortOption < 1 || sortOption > 5 || sortCriteria < 1 || sortCriteria > 5) {
			printf("Error: para ordenar debes usar -a <1..5> y -c <1..5>\n\n");
			print_usage(argv[0]);
			return 1;
		}

		if ((players = load_players("build/db/players.csv", &n)) == NULL) {
			return 1;
		}

		printf(LIGHT_BLUE "\nOriginal file:\n" RESET);
		print_player_array_more(players, n);

		if (sortCriteria == 1) comp_ptr = compare_id;
		else if (sortCriteria == 2) comp_ptr = compare_name;
		else if (sortCriteria == 3) comp_ptr = compare_team;
		else if (sortCriteria == 4) comp_ptr = compare_score;
		else if (sortCriteria == 5) comp_ptr = compare_competitions;

		if (sortOption == 1) {
			swap_sort(players, n, comp_ptr);
		} else if (sortOption == 2) {
			insertion_sort(players, n, comp_ptr);
		} else if (sortOption == 3) {
			selection_sort(players, n, comp_ptr);
		} else if (sortOption == 4) {
			cocktail_shaker_sort(players, n, comp_ptr);
		} else if (sortOption == 5) {
			quick_sort(players, 0, n - 1, comp_ptr, 4);
		}

		printf(LIGHT_BLUE "\nSorted file (Ascending):\n" RESET);
		print_player_array_more(players, n);
		free(players);
		return 0;
	}

	if (action == 'f') {
		if (searchOption < 1 || searchOption > 2 || searchId < 0) {
			printf("Error: para buscar debes usar -b <1|2> -i <id>\n\n");
			print_usage(argv[0]);
			return 1;
		}

		if ((players = load_players("build/db/players.csv", &n)) == NULL) {
			return 1;
		}

		printf(LIGHT_GREEN "\nCurrent file:\n" RESET);
		print_player_array_more(players, n);

		Player target = {searchId, "", "", 0.0, 0};

		if (searchOption == 1) {
			result = linear_search(players, n, &target, compare_id);

			if (result == n) {
				printf(BG_RED "Player with ID %d was not found." RESET "\n\n", searchId);
			} else {
				printf(BG_GREEN "Player found at index %d via Linear Search:" RESET "\n\n", result + 1);
				print_player(&players[result]);
			}
		} else {
			cocktail_shaker_sort(players, n, compare_id);

			printf(LIGHT_GREEN "\nOrdered array (by ID) used for binary search:\n" RESET);
			print_player_array_more(players, n);

			result = binary_search(players, 0, n - 1, &target);

			if (result == -1) {
				printf(BG_RED "Player with ID %d was not found." RESET "\n\n", searchId);
			} else {
				printf(BG_GREEN "Player found at index %d via Binary Search:" RESET "\n\n", result + 1);
				print_player(&players[result]);
			}
		}

		free(players);
		return 0;
	}

	if (action == 'e') {
		run_experiment();
		return 0;
	}

	if (action == 'x') {
		return 0;
	}

	print_usage(argv[0]);
	return 1;
}

static void print_usage(const char *progname){
	printf(LIGHT_BLUE "╔════════════════════════════════════════════════════════════╗\n");
	printf(LIGHT_BLUE "-" WHITE "                      CONQUER CLI USAGE                   " LIGHT_BLUE "  -\n");
	printf(LIGHT_BLUE "╚════════════════════════════════════════════════════════════╝\n" RESET);

	printf("\n" DARK_GRAY "Uso rapido:\n" RESET);
	printf("  " YELLOW "%s " DARK_YELLOW "-g " LIGHT_GRAY "<cantidad> " DARK_YELLOW "-t " LIGHT_GRAY "<1|2|3>" RESET "       " YELLOW "Generar CSV\n" RESET, progname);
	printf("  " YELLOW "%s " ORANGE "-r" RESET "                             " ORANGE "Leer CSV actual\n" RESET, progname);
	printf("  " YELLOW "%s " DARK_BLUE "-s " LIGHT_GRAY "-a <1|2|3|4> -c <1|2|3|4|5>" RESET " " LIGHT_BLUE "Ordenar CSV\n" RESET, progname);
	printf("  " YELLOW "%s " DARK_GREEN "-f " LIGHT_GRAY "-b <1|2> -i <id>" RESET "            " LIGHT_GREEN "Buscar por ID\n" RESET, progname);
	printf("  " YELLOW "%s " PURPLE "-e" RESET "                             " MAGENTA "Ejecutar experimento\n" RESET, progname);
	printf("  " YELLOW "%s " DARK_RED "-x" RESET "                             " LIGHT_RED "Salir\n" RESET, progname);
	printf("  " YELLOW "%s " WHITE "-h" RESET "                             " WHITE "Mostrar ayuda\n" RESET, progname);

	printf("\n" DARK_GRAY "Opciones:\n" RESET);
	printf("  " DARK_YELLOW "-g, --generate <cantidad>" RESET "         " LIGHT_GRAY "                 Genera un CSV con la cantidad indicada\n" RESET);
	printf("  " DARK_YELLOW "-t, --type <1|2|3>" RESET "                " LIGHT_GRAY "                 Tipo de generacion: 1=sorted, 2=inverse, 3=shuffled\n" RESET);
	printf("  " ORANGE "-r, --read" RESET "                        " LIGHT_GRAY "                 Lee e imprime el CSV actual\n" RESET);
	printf("  " DARK_BLUE "-s, --sort" RESET "                        " LIGHT_GRAY "                 Ordena el CSV actual\n" RESET);
	printf("  " DARK_BLUE "-a, --algorithm <1|2|3|4|5>" RESET "         " LIGHT_GRAY "                 Algoritmo: 1=swap, 2=insertion, 3=selection, 4=cocktail, 5=quick\n" RESET);
	printf("  " DARK_BLUE "-c, --criteria <1|2|3|4|5>" RESET "        " LIGHT_GRAY "                 Criterio: 1=ID, 2=Name, 3=Team, 4=Score, 5=Competitions\n" RESET);
	printf("  " DARK_GREEN "-f, --find" RESET "                        " LIGHT_GRAY "                 Busca un jugador por ID\n" RESET);
	printf("  " DARK_GREEN "-b, --binary <1|2>" RESET "                " LIGHT_GRAY "                 Busqueda: 1=linear, 2=binary\n" RESET);
	printf("  " DARK_GREEN "-i, --id <id>" RESET "                     " LIGHT_GRAY "                 ID del jugador a buscar\n" RESET);
	printf("  " PURPLE "-e, --experiment" RESET "                  " LIGHT_GRAY "                 Ejecuta el experimento\n" RESET);
	printf("  " DARK_RED "-x, --exit" RESET "                        " LIGHT_GRAY "                 Sale del programa\n" RESET);
	printf("  " WHITE "-h, --help" RESET "                        " LIGHT_GRAY "                 Muestra esta ayuda\n\n" RESET);
}
