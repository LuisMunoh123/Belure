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
	{"threshold-experiment", no_argument, 0, 'T'},
	{"type", required_argument, 0, 't'},
	{"algorithm", required_argument, 0, 'a'},
	{"criteria", required_argument, 0, 'c'},
	{"id", required_argument, 0, 'i'},
	{"kth", required_argument, 0, 'j'},
	{"topcount", required_argument, 0, 'p'},
	{"score", required_argument, 0, 'q'},
	{"score-range", no_argument, 0, 'R'},
	{"min-score", required_argument, 0, 'm'},
	{"max-score", required_argument, 0, 'M'},
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
	int kthValue = -1;
	int topCount = -1;
	float minScore = 0.0f;
	float maxScore = 0.0f;
	float exactScore = 0.0f;
	int minScoreSet = 0;
	int maxScoreSet = 0;
	int exactScoreSet = 0;

	srand(time(0));

	while ((opt = getopt_long(argc, argv, "g:rsf:eht:a:c:i:j:p:q:Rm:M:", long_options, NULL)) != -1) {
		switch (opt) {
			// generacion de datos
			case 'g':
				action = 'g';
				action_count++;
				n = atoi(optarg);
				break;
			// leer el archivo csv actual
			case 'r':
			// ordenamiento segun algoritmo y criterio
			case 's':
			// ejecutar experimento
			case 'e':
			action = (char)opt;
			action_count++;
			break;

			// busqueda por ID
			case 'f':
				action = 'f';
				action_count++;
				searchOption = parse_search_algorithm(optarg);
				break;

			// tipo de generacion (ordenado, invertido, desordenado)
			case 't':
				generationType = parse_generation_type(optarg);
				break;

			// tipo de algoritmo (swap, insertion, quick, etc.)
			case 'a':
				sortOption = parse_sort_algorithm(optarg);
				break;

			// tipo de criterio (id, nombre, puntaje, etc.)
			case 'c':
				sortCriteria = parse_sort_criteria(optarg);
				break;
			
			// id del jugador buscado
			case 'i':
				searchId = atoi(optarg);
				break;

			// el k-esimo mejor deportista
			case 'j':
				if (action != 'j') {
					action = 'j';
					action_count++;
				}
				kthValue = atoi(optarg);
				break;

			// ranking de los mejores N deportistas
			case 'p':
				if (action != 'n') {
					action = 'n';
					action_count++;
				}
				topCount = atoi(optarg);
				break;

			// busca todos los jugadores con el mismo puntaje indicado
			case 'q':
				action = 'Q';
				action_count++;
				exactScore = (float)atof(optarg);
				exactScoreSet = 1;
				break;

			// Busqueda de jugadores en un rango establecido de puntajes
			case 'R':
				action = 'R';
				action_count++;
				break;

			// Puntaje minimo del rango
			case 'm':
				minScore = (float)atof(optarg);
				minScoreSet = 1;
				break;

			// Puntaje maximo del rango
			case 'M':
				maxScore = (float)atof(optarg);
				maxScoreSet = 1;
				break;

			// muestra el menú de ayuda o ejemplo
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

	// generacion de datos
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

	// leer el archivo csv actual
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

	// ordenamiento segun algoritmo y criterio
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
		} else if (sortOption == MERGE) {
			merge_sort_classic(players, 0, n - 1, comp_ptr);
		} else if (sortOption == MERGE_OPTIMIZED) {
			merge_sort_optimized(players, 0, n - 1, MERGE_THRESHOLD, comp_ptr);
		}

		printf(LIGHT_BLUE "\nSorted file (Ascending):\n" RESET);
		print_player_array_more(players, n);
		free(players);
		return 0;
	}

	// busqueda por ID
	if (action == 'f') {
		if (searchOption == SEARCH_INVALID || searchId < 0) {
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

			result = binary_search_iterative(players, 0, n - 1, &target, compare_id);

			if (result == -1) {
				printf(BG_RED "Player with ID %d was not found." RESET "\n\n", searchId);
			} else {
				printf(BG_GREEN "Player found at index %d via Binary Search:" RESET "\n\n", result + 1);
				print_player(&players[result]);
			}
		} else if (searchOption == BINARY_RECURSIVE) {
			quick_sort(players, 0, n - 1, compare_id);

			printf(LIGHT_GREEN "\nOrdered array (by ID) used for recursive binary search:\n" RESET);
			print_player_array_more(players, n);

			result = binary_search_recursive(players, 0, n - 1, &target, compare_id);

			if (result == -1) {
				printf(BG_RED "Player with ID %d was not found." RESET "\n\n", searchId);
			} else {
				printf(BG_GREEN "Player found at index %d via Recursive Binary Search:" RESET "\n\n", result + 1);
				print_player(&players[result]);
			}
		} else if (searchOption == EXPONENTIAL) {
			quick_sort(players, 0, n - 1, compare_id);

			printf(LIGHT_GREEN "\nOrdered array (by ID) used for exponential search:\n" RESET);
			print_player_array_more(players, n);

			result = exponential_search(players, n, &target, compare_id);

			if (result == -1) {
				printf(BG_RED "Player with ID %d was not found." RESET "\n\n", searchId);
			} else {
				printf(BG_GREEN "Player found at index %d via Exponential Search:" RESET "\n\n", result + 1);
				print_player(&players[result]);
			}
		} else if (searchOption == INTERPOLATION) {
			quick_sort(players, 0, n - 1, compare_id);

			printf(LIGHT_GREEN "\nOrdered array (by ID) used for interpolation search:\n" RESET);
			print_player_array_more(players, n);

			result = interpolation_search(players, 0, n - 1, searchId);

			if (result == -1) {
				printf(BG_RED "Player with ID %d was not found." RESET "\n\n", searchId);
			} else {
				printf(BG_GREEN "Player found at index %d via Interpolation Search:" RESET "\n\n", result + 1);
				print_player(&players[result]);
			}
		} else {
			printf("Error: algoritmo de busqueda no soportado.\n");
			free(players);
			return 1;
		}

		free(players);
		return 0;
	}

	// el k-esimo mejor deportista
	if (action == 'j') {
		if (kthValue <= 0) {
			printf("Error: para seleccion debes usar -j <k>\n\n");
			print_usage(argv[0]);
			return 1;
		}

		if ((players = load_players("build/db/players.csv", &n)) == NULL) {
			return 1;
		}

		if (kthValue > n) {
			print_error(103, "Players to select", NULL);
			free(players);
			return 1;
		}

		printf(LIGHT_GREEN "\nCurrent file:\n" RESET);
		print_player_array_more(players, n);

		Player target = quick_select(players, 0, n - 1, n - kthValue, compare_score);

		printf(BG_GREEN "The %d best player is: " RESET "\n\n", kthValue);
		print_player(&target);

		free(players);
		return 0;
	}

	// ranking de los mejores N deportistas
	if (action == 'n') {
		if (topCount <= 0) {
			printf("Error: para ranking debes usar -p <N>\n\n");
			print_usage(argv[0]);
			return 1;
		}

		if ((players = load_players("build/db/players.csv", &n)) == NULL) {
			return 1;
		}

		if (topCount > n) {
			print_error(103, "Players to rank", NULL);
			free(players);
			return 1;
		}

		printf(LIGHT_GREEN "\nCurrent file:\n" RESET);
		print_player_array_more(players, n);

		quick_sort(players, 0, n - 1, compare_score);

		printf(BG_GREEN "Top %d players by score:" RESET "\n\n", topCount);

		print_player_array_more(&players[n-1-topCount], topCount);

		free(players);
		return 0;
	}

	// busqueda de jugadores en un rango establecido de puntajes
	if (action == 'R') {
		if (!minScoreSet || !maxScoreSet) {
			printf("Error: para rango de puntajes debes usar -R -m <min> -M <max>\n\n");
			print_usage(argv[0]);
			return 1;
		}

		if (minScore > maxScore) {
			printf("Error: el puntaje minimo no puede ser mayor que el maximo.\n\n");
			return 1;
		}

		if ((players = load_players("build/db/players.csv", &n)) == NULL) {
			return 1;
		}

		printf(LIGHT_GREEN "\nCurrent file:\n" RESET);
		print_player_array_more(players, n);

		quick_sort(players, 0, n - 1, compare_score);

		printf(LIGHT_GREEN "\nOrdered array (by score) used for range binary search:\n" RESET);
		print_player_array_more(players, n);

		Player minTarget = {0, "", "", minScore, 0};
		Player maxTarget = {0, "", "", maxScore, 0};
		int first = binary_search_lower_bound(players, 0, n - 1, &minTarget, compare_score);
		int last = binary_search_upper_bound(players, 0, n - 1, &maxTarget, compare_score);

		if (first == -1 || last == -1 || first > last) {
			printf(BG_RED "No players were found with score in range [%.1f, %.1f]." RESET "\n\n", minScore, maxScore);
			free(players);
			return 0;
		}

		int rangeCount = last - first + 1;

		printf(BG_GREEN "Players with score in range [%.1f, %.1f] via Range Binary Search:" RESET "\n\n", minScore, maxScore);
		print_player_array(&players[first], rangeCount);
		printf(LIGHT_GREEN "Total players found: %d\n\n" RESET, rangeCount);

		free(players);
		return 0;
	}

	// busca todos los jugadores con el mismo puntaje indicado
	if (action == 'Q') {
		if (!exactScoreSet) {
			printf("Error: para buscar un puntaje exacto debes usar -q <score>\n\n");
			print_usage(argv[0]);
			return 1;
		}

		if ((players = load_players("build/db/players.csv", &n)) == NULL) {
			return 1;
		}

		printf(LIGHT_GREEN "\nCurrent file:\n" RESET);
		print_player_array_more(players, n);

		quick_sort(players, 0, n - 1, compare_score);

		printf(LIGHT_GREEN "\nOrdered array (by score) used for exact score range search:\n" RESET);
		print_player_array_more(players, n);

		Player exactTarget = {0, "", "", exactScore, 0};
		int first = binary_search_first(players, 0, n - 1, &exactTarget, compare_score);

		if (first == -1) {
			printf(BG_RED "No players were found with score %.1f." RESET "\n\n", exactScore);
			free(players);
			return 0;
		}

		int last = binary_search_last(players, first, n - 1, &exactTarget, compare_score);
		int rangeCount = last - first + 1;

		printf(BG_GREEN "Players with exact score %.1f via Range Binary Search:" RESET "\n\n", exactScore);
		print_player_array(&players[first], rangeCount);
		printf(LIGHT_GREEN "Total players found: %d\n\n" RESET, rangeCount);

		free(players);
		return 0;
	}

	// ejecutar experimento
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
	printf("  " YELLOW "%s " MAGENTA "-j <k>" RESET "                         " MAGENTA "        Mostrar el k-esimo mejor deportista\n" RESET, progname);
	printf("  " YELLOW "%s " MAGENTA "-p <N>" RESET "                         " MAGENTA "        Mostrar ranking top N\n" RESET, progname);
	printf("  " YELLOW "%s " MAGENTA "-q <score>" RESET "                     " MAGENTA "        Mostrar jugadores con puntaje exacto\n" RESET, progname);
	printf("  " YELLOW "%s " MAGENTA "-R -m <min> -M <max>" RESET "            " MAGENTA "       Mostrar jugadores por rango de puntaje\n" RESET, progname);
	printf("  " YELLOW "%s " PURPLE "-e" RESET "                             " MAGENTA "        Ejecutar experimento\n" RESET, progname);
	printf("  " YELLOW "%s " WHITE "-h" RESET "                             " WHITE "        Mostrar ayuda\n" RESET, progname);

	printf("\n" DARK_GRAY "Opciones:\n" RESET);
	printf("  " DARK_YELLOW "-g, --generate <cantidad>" RESET "         " LIGHT_GRAY "                         Genera un CSV con la cantidad indicada\n" RESET);
	printf("  " DARK_YELLOW "-t, --type <generate type>" RESET "                " LIGHT_GRAY "                 Tipo de generacion: sorted, inverse, shuffled\n" RESET);
	printf("  " ORANGE "-r, --read" RESET "                        " LIGHT_GRAY "                         Lee e imprime el CSV actual\n" RESET);
	printf("  " DARK_BLUE "-s, --sort" RESET "                        " LIGHT_GRAY "                         Ordena el CSV actual\n" RESET);
	printf("  " DARK_BLUE "-a, --algorithm <sort type>" RESET "         " LIGHT_GRAY "                       Algoritmo: swap, insertion, selection, cocktail, quick, merge, merge-optimized\n" RESET);
	printf("  " DARK_BLUE "-c, --criteria <criteria>" RESET "        " LIGHT_GRAY "                          Criterio: id, name, team, score, competitions\n" RESET);
	printf("  " DARK_GREEN "-f, --find <search type>" RESET "                        " LIGHT_GRAY "           Busca un jugador por ID, busqueda: linear, binary,\n binary-recursive, exponential, interpolation\n" RESET);
	printf("  " DARK_GREEN "-i, --id <id>" RESET "                     " LIGHT_GRAY "                         ID del jugador a buscar\n" RESET);
	printf("  " MAGENTA "-j, --kth <k>" RESET "                     " LIGHT_GRAY "                         Muestra el k-esimo mejor deportista\n" RESET);
	printf("  " MAGENTA "-p, --topcount <N>" RESET "               " LIGHT_GRAY "                          Muestra el ranking de los mejores N deportistas\n" RESET);
	printf("  " MAGENTA "-q, --score <score>" RESET "              " LIGHT_GRAY "                          Busca jugadores con puntaje exacto repetido\n" RESET);
	printf("  " MAGENTA "-R, --score-range" RESET "                " LIGHT_GRAY "                          Activa busqueda binaria por rango de puntajes\n" RESET);
	printf("  " MAGENTA "-m, --min-score <min>" RESET "            " LIGHT_GRAY "                          Puntaje minimo del rango\n" RESET);
	printf("  " MAGENTA "-M, --max-score <max>" RESET "            " LIGHT_GRAY "                          Puntaje maximo del rango\n" RESET);
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
	if (strcmp(value, "merge") == 0) return MERGE;
	if (strcmp(value, "merge-optimized") == 0) return MERGE_OPTIMIZED;
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
	if (strcmp(value, "binary-recursive") == 0) return BINARY_RECURSIVE;
	if (strcmp(value, "exponential") == 0) return EXPONENTIAL;
	if (strcmp(value, "interpolation") == 0) return INTERPOLATION;
	return SEARCH_INVALID;
}
