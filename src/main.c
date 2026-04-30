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
#include "selection.h"
#include "utilities.h"
#include "player.h" 
#include "generate_exec_times.h"

static struct option long_options[] = {
	{"generate", required_argument, 0, 'g'},
	{"read", no_argument, 0, 'r'},
	{"sort", no_argument, 0, 's'},
	{"find", required_argument, 0, 'f'},
	{"experiment", no_argument, 0, 'e'},
	{"type", required_argument, 0, 't'},
	{"algorithm", required_argument, 0, 'a'},
	{"criteria", required_argument, 0, 'c'},
	{"id", required_argument, 0, 'i'},
	{"help", no_argument, 0, 'h'},
	{0, 0, 0, 0}
};

static void print_usage(const char *progname);
static GenerationType parse_generation_type(const char *value);
static SortAlgorithm parse_sort_algorithm(const char *value);
static SortCriteria parse_sort_criteria(const char *value);
static SearchAlgorithm parse_search_algorithm(const char *value);

int main(int argc, char *argv[])
{
	int opt;
	char action = '\0';
	int action_count = 0;

	GenerationType generationType = GEN_INVALID;
	Player *players = NULL;
	int n = -1;

	SortAlgorithm sortOption = SORT_INVALID;
	SortCriteria sortCriteria = CRIT_INVALID;

	SearchAlgorithm searchOption = SEARCH_INVALID;
	int searchId = -1;
	int result;

	srand(time(0));

	while ((opt = getopt_long(argc, argv, "g:rsf:eht:a:c:i:", long_options, NULL)) != -1) {
		switch (opt) {
			case 'g':
				action = 'g';
				action_count++;
				n = atoi(optarg);
				break;

			case 'r':
			case 's':
			case 'e':
			action = (char)opt;
			action_count++;
			break;

			case 'f':
				action = 'f';
				action_count++;
				searchOption = parse_search_algorithm(optarg);
				break;

			case 't':
				generationType = parse_generation_type(optarg);
				break;

			case 'a':
				sortOption = parse_sort_algorithm(optarg);
				break;

			case 'c':
				sortCriteria = parse_sort_criteria(optarg);
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
		if (generationType == GEN_INVALID) {
			printf("Error: para generar debes usar -g <cantidad> y -t <generate type>\n\n");
			print_usage(argv[0]);
			return 1;
		}

		if (generationType == SORTED) {
			printf(YELLOW "\nGenerating SORTED array...\n");
		} else if (generationType == INVERSE) {
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

		if (sortOption == SORT_INVALID || sortCriteria == CRIT_INVALID) {
			printf("Error: para ordenar debes usar -a <sort type> y -c <criteria>\n\n");
			print_usage(argv[0]);
			return 1;
		}

		if ((players = load_players("build/db/players.csv", &n)) == NULL) {
			return 1;
		}

		printf(LIGHT_BLUE "\nOriginal file:\n" RESET);
		print_player_array_more(players, n);

		if (sortCriteria == ID) comp_ptr = compare_id;
		else if (sortCriteria == NAME) comp_ptr = compare_name;
		else if (sortCriteria == TEAM) comp_ptr = compare_team;
		else if (sortCriteria == SCORE) comp_ptr = compare_score;
		else if (sortCriteria == COMPETITIONS) comp_ptr = compare_competitions;

		if (sortOption == SWAP) {
			swap_sort(players, n, comp_ptr);
		} else if (sortOption == INSERTION) {
			insertion_sort(players, n, comp_ptr);
		} else if (sortOption == SELECTION) {
			selection_sort(players, n, comp_ptr);
		} else if (sortOption == COCKTAIL) {
			cocktail_shaker_sort(players, n, comp_ptr);
		} else if (sortOption == QUICK) {
			quick_sort(players, 0, n - 1, comp_ptr);
		}

		printf(LIGHT_BLUE "\nSorted file (Ascending):\n" RESET);
		print_player_array_more(players, n);
		free(players);
		return 0;
	}

	if (action == 'f') {
		if (searchOption == SEARCH_INVALID) {
			printf("Error: para buscar debes usar -f <search type> -i <id>\n\n");
			print_usage(argv[0]);
			return 1;
		}

		if ((players = load_players("build/db/players.csv", &n)) == NULL) {
			return 1;
		}

		printf(LIGHT_GREEN "\nCurrent file:\n" RESET);
		print_player_array_more(players, n);

		Player target = {searchId, "", "", 0.0, 0};

		if (searchOption == LINEAR) {
			result = linear_search(players, n, &target, compare_id);

			if (result == n) {
				printf(BG_RED "Player with ID %d was not found." RESET "\n\n", searchId);
			} else {
				printf(BG_GREEN "Player found at index %d via Linear Search:" RESET "\n\n", result + 1);
				print_player(&players[result]);
			}
		} else if (searchOption == BINARY) {
			quick_sort(players, 0, n - 1, compare_id);

			printf(LIGHT_GREEN "\nOrdered array (by ID) used for binary search:\n" RESET);
			print_player_array_more(players, n);

			result = binary_search(players, 0, n - 1, &target);

			if (result == -1) {
				printf(BG_RED "Player with ID %d was not found." RESET "\n\n", searchId);
			} else {
				printf(BG_GREEN "Player found at index %d via Binary Search:" RESET "\n\n", result + 1);
				print_player(&players[result]);
			}
		} else {
			if (searchId > n) {
				print_error(103, "Players to select", NULL);
				return 1;
			}
			// Buscamos el searched-id-ésimo mejor jugador segun el score
			target = quick_select(players, 0, n - 1, n - searchId, compare_score);
			printf(BG_GREEN "The %d best player is: " RESET "\n\n", searchId);
			print_player(&target);
		}

		free(players);
		return 0;
	}

	if (action == 'e') {
		run_experiment();
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
	printf("  " YELLOW "%s " DARK_YELLOW "-g " LIGHT_GRAY "<cantidad> " DARK_YELLOW "-t " LIGHT_GRAY "<generate type>" RESET "       " YELLOW "Generar CSV\n" RESET, progname);
	printf("  " YELLOW "%s " ORANGE "-r" RESET "                             " ORANGE "        Leer CSV actual\n" RESET, progname);
	printf("  " YELLOW "%s " DARK_BLUE "-s " LIGHT_GRAY "-a <sort type> -c <criteria>" RESET " " LIGHT_BLUE "       Ordenar CSV\n" RESET, progname);
	printf("  " YELLOW "%s " DARK_GREEN "-f " LIGHT_GRAY "<search type> -i <id>" RESET "            " LIGHT_GREEN "   Buscar por ID\n" RESET, progname);
	printf("  " YELLOW "%s " PURPLE "-e" RESET "                             " MAGENTA "        Ejecutar experimento\n" RESET, progname);
	printf("  " YELLOW "%s " WHITE "-h" RESET "                             " WHITE "        Mostrar ayuda\n" RESET, progname);

	printf("\n" DARK_GRAY "Opciones:\n" RESET);
	printf("  " DARK_YELLOW "-g, --generate <cantidad>" RESET "         " LIGHT_GRAY "                         Genera un CSV con la cantidad indicada\n" RESET);
	printf("  " DARK_YELLOW "-t, --type <generate type>" RESET "                " LIGHT_GRAY "                 Tipo de generacion: sorted, inverse, shuffled\n" RESET);
	printf("  " ORANGE "-r, --read" RESET "                        " LIGHT_GRAY "                         Lee e imprime el CSV actual\n" RESET);
	printf("  " DARK_BLUE "-s, --sort" RESET "                        " LIGHT_GRAY "                         Ordena el CSV actual\n" RESET);
	printf("  " DARK_BLUE "-a, --algorithm <sort type>" RESET "         " LIGHT_GRAY "                       Algoritmo: swap, insertion, selection, cocktail, quick\n" RESET);
	printf("  " DARK_BLUE "-c, --criteria <criteria>" RESET "        " LIGHT_GRAY "                          Criterio: id, name, team, score, competitions\n" RESET);
	printf("  " DARK_GREEN "-f, --find <search type>" RESET "                        " LIGHT_GRAY "           Busca un jugador por ID, busqueda: linear, binary, quick\n" RESET);
	printf("  " DARK_GREEN "-i, --id <id>" RESET "                     " LIGHT_GRAY "                         ID del jugador a buscar\n" RESET);
	printf("  " PURPLE "-e, --experiment" RESET "                  " LIGHT_GRAY "                         Ejecuta el experimento\n" RESET);
	printf("  " WHITE "-h, --help" RESET "                        " LIGHT_GRAY "                         Muestra esta ayuda\n\n" RESET);
}

static GenerationType parse_generation_type(const char *value){
	if (strcmp(value, "sorted") == 0) return SORTED;
	if (strcmp(value, "inverse") == 0) return INVERSE;
	if (strcmp(value, "shuffled") == 0) return SHUFFLED;
	return GEN_INVALID;
}

static SortAlgorithm parse_sort_algorithm(const char *value){
	if (strcmp(value, "swap") == 0) return SWAP;
	if (strcmp(value, "insertion") == 0) return INSERTION;
	if (strcmp(value, "selection") == 0) return SELECTION;
	if (strcmp(value, "cocktail") == 0) return COCKTAIL;
	if (strcmp(value, "quick") == 0) return QUICK;
	return SORT_INVALID;
}

static SortCriteria parse_sort_criteria(const char *value){
	if (strcmp(value, "id") == 0) return ID;
	if (strcmp(value, "name") == 0) return NAME;
	if (strcmp(value, "team") == 0) return TEAM;
	if (strcmp(value, "score") == 0) return SCORE;
	if (strcmp(value, "competitions") == 0) return COMPETITIONS;
	return CRIT_INVALID;
}

static SearchAlgorithm parse_search_algorithm(const char *value){
	if (strcmp(value, "linear") == 0) return LINEAR;
	if (strcmp(value, "binary") == 0) return BINARY;
	if (strcmp(value, "quick") == 0) return QUICKSELECT;
	return SEARCH_INVALID;
}
