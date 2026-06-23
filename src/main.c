/**
 * @file main.c
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Main program functions.
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
#include "experiment_config.h"
#include "search_experiment.h"
#include "sort_experiment.h"
#include "greedy.h"
#include "dynamic_programming.h"
#include "team_experiment.h"

static struct option long_options[] = {
	{"generate", required_argument, 0, 'g'},
	{"read", no_argument, 0, 'r'},
	{"sort", no_argument, 0, 's'},
	{"find", optional_argument, 0, 'f'},
	{"experiment", no_argument, 0, 'e'},
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
	{"greedy", no_argument, 0, 'G'},
	{"dynamic-programming", required_argument, 0, 'd'},
	{"budget", required_argument, 0, 'b'},
	{"help", no_argument, 0, 'h'},
	{0, 0, 0, 0}
};

static void print_usage(const char *progname);
static GenerationType parse_generation_type(const char *value);
static SortAlgorithm parse_sort_algorithm(const char *value);
static SortCriteria parse_sort_criteria(const char *value);
static SearchAlgorithm parse_search_algorithm(const char *value);
static DPAlgorithm parse_dp_algorithm(const char *value);
static void print_dp_result(DPResult *result, int budget, const char *method_name);
static int run_experiment_menu(void);

int main(int argc, char *argv[])
{
	int opt;
	char action = '\0';
	int action_count = 0;

	GenerationType generationType = GEN_INVALID;
	Player *players = NULL;
	int n = -1;

	SortAlgorithm sortOption = QUICK;
	SortCriteria sortCriteria = CRIT_INVALID;
	int sortOptionSpecified = 1;
	int sortCriteriaSpecified = 0;

	SearchAlgorithm searchOption = INTERPOLATION;
	int searchOptionSpecified = 1;
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
	DPAlgorithm dpOption = DP_INVALID;
	int budget = -1;
	int budgetSet = 0;

	srand(time(0));

	while ((opt = getopt_long(argc, argv, "g:rsf::eht:a:c:i:j:p:q:Rm:M:d:b:G", long_options, NULL)) != -1) {
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
				if (optarg != NULL) {
					searchOption = parse_search_algorithm(optarg);
					searchOptionSpecified = 1;
				}
				break;

			// tipo de generacion (ordenado, invertido, desordenado)
			case 't':
				generationType = parse_generation_type(optarg);
				break;

			// tipo de algoritmo (swap, insertion, quick, etc.)
			case 'a':
				sortOptionSpecified = 1;
				sortOption = parse_sort_algorithm(optarg);
				break;

			// tipo de criterio (id, nombre, puntaje, etc.)
			case 'c':
				sortCriteriaSpecified = 1;
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
				if (action != 'p') {
					action = 'p';
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

			case 'G':
    			action = 'G';
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

			// metodo de programacion dinamica
			case 'd':
				action = 'd';
				action_count++;
				dpOption = parse_dp_algorithm(optarg);
				break;

			// presupuesto maximo
			case 'b':
				budget = atoi(optarg);
				budgetSet = 1;
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

	if (action == 'f' && !searchOptionSpecified && optind < argc && argv[optind][0] != '-') {
		searchOption = parse_search_algorithm(argv[optind]);
		searchOptionSpecified = 1;
		optind++;
	}

	if (action_count != 1) {
		print_error(104, NULL, NULL);
		printf("\n");
		print_usage(argv[0]);
		return 1;
	}
	if (action == 'G') {
    	greedy_experiment();
    	return 0;
	}
	if (action == 'g') {
		if (generationType == GEN_INVALID) {
			print_error(105, NULL, NULL);
			printf("\n");
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

		if (sortOptionSpecified && sortOption == SORT_INVALID) {
			print_error(106, NULL, NULL);
			printf("\n");
			print_usage(argv[0]);
			return 1;
		}

		if (sortCriteriaSpecified && sortCriteria == CRIT_INVALID) {
			print_error(107, NULL, NULL);
			printf("\n");
			print_usage(argv[0]);
			return 1;
		}

		if (!sortOptionSpecified) sortOption = QUICK;
		if (!sortCriteriaSpecified) sortCriteria = ID;

		if ((players = load_players("build/db/players.csv", &n)) == NULL) {
			return 1;
		}

			if (sortCriteria == ID) comp_ptr = compare_id;
			else if (sortCriteria == NAME) comp_ptr = compare_name;
			else if (sortCriteria == TEAM) comp_ptr = compare_team;
		else if (sortCriteria == SCORE) comp_ptr = compare_score;
		else if (sortCriteria == COMPETITIONS) comp_ptr = compare_competition;

		if (sortOption == SWAP) {
			printf(BG_GREEN "Sorting Players via Swap Sort:" RESET "\n");
			swap_sort(players, n, comp_ptr);
		} else if (sortOption == INSERTION) {
			printf(BG_GREEN "Sorting Players via Insertion Sort:" RESET "\n");
			insertion_sort(players, n, comp_ptr);
		} else if (sortOption == SELECTION) {
			printf(BG_GREEN "Sorting Players via Selection Sort:" RESET "\n");
			selection_sort(players, n, comp_ptr);
		} else if (sortOption == COCKTAIL) {
			printf(BG_GREEN "Sorting Players via Cocktail Shaker Sort:" RESET "\n");
			cocktail_shaker_sort(players, n, comp_ptr);
		} else if (sortOption == QUICK) {
			printf(BG_GREEN "Sorting Players via Quick Sort:" RESET "\n");
			quick_sort(players, 0, n - 1, comp_ptr);
		} else if (sortOption == MERGE) {
			printf(BG_GREEN "Sorting Players via Merge Sort:" RESET "\n");
			merge_sort_classic(players, 0, n - 1, comp_ptr);
		} else if (sortOption == MERGE_OPTIMIZED) {
			printf(BG_GREEN "Sorting Players via Merge Sort (Optimized):" RESET "\n");
			merge_sort_optimized(players, 0, n - 1, MERGE_THRESHOLD, comp_ptr);
		}

		printf(LIGHT_BLUE "\nSorted file (Ascending):\n" RESET);
		print_player_array_more(players, n);
		free(players);
		return 0;
	}

	if (action == 'f') {
		if (searchOptionSpecified && searchOption == SEARCH_INVALID) {
			print_error(108, NULL, NULL);
			printf("\n");
			print_usage(argv[0]);
			return 1;
		}

		if (searchId < 0) {
			print_error(109, NULL, NULL);
			printf("\n");
			print_usage(argv[0]);
			return 1;
		}

		if (!searchOptionSpecified) searchOption = INTERPOLATION;

		if ((players = load_players("build/db/players.csv", &n)) == NULL) {
			return 1;
		}

			Player target = {searchId, "", "", 0.0, 0, 0};
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

				result = binary_search_iterative(players, 0, n - 1, &target, compare_id);

			if (result == -1) {
				printf(BG_RED "Player with ID %d was not found." RESET "\n\n", searchId);
			} else {
				printf(BG_GREEN "Player found at index %d via Binary Search:" RESET "\n\n", result + 1);
				print_player(&players[result]);
			}
			} else if (searchOption == BINARY_RECURSIVE) {
				quick_sort(players, 0, n - 1, compare_id);

				result = binary_search_recursive(players, 0, n - 1, &target, compare_id);

			if (result == -1) {
				printf(BG_RED "Player with ID %d was not found." RESET "\n\n", searchId);
			} else {
				printf(BG_GREEN "Player found at index %d via Recursive Binary Search:" RESET "\n\n", result + 1);
				print_player(&players[result]);
			}
			} else if (searchOption == EXPONENTIAL) {
				quick_sort(players, 0, n - 1, compare_id);

				result = exponential_search(players, n, &target, compare_id);

			if (result == -1) {
				printf(BG_RED "Player with ID %d was not found." RESET "\n\n", searchId);
			} else {
				printf(BG_GREEN "Player found at index %d via Exponential Search:" RESET "\n\n", result + 1);
				print_player(&players[result]);
			}
			} else if (searchOption == INTERPOLATION) {
				quick_sort(players, 0, n - 1, compare_id);

				result = interpolation_search(players, 0, n - 1, searchId);

			if (result == -1) {
				printf(BG_RED "Player with ID %d was not found." RESET "\n\n", searchId);
			} else {
				printf(BG_GREEN "Player found at index %d via Interpolation Search:" RESET "\n\n", result + 1);
				print_player(&players[result]);
			}
			} else {
				print_error(110, NULL, NULL);
				free(players);
				return 1;
			}

		free(players);
		return 0;
	}

	if (action == 'j') {
		if (kthValue <= 0) {
			print_error(111, NULL, NULL);
			printf("\n");
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

			Player target = quick_select(players, 0, n - 1, n - kthValue, compare_score);

		printf(BG_GREEN "The %d best player is: " RESET "\n\n", kthValue);
		print_player(&target);

		free(players);
		return 0;
	}

	if (action == 'p') {
		if (topCount <= 0) {
			print_error(112, NULL, NULL);
			printf("\n");
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

			quick_select(players, 0, n - 1, n - topCount, compare_score);
			quick_sort(&players[n - topCount], 0, topCount - 1, compare_score);

		printf(BG_GREEN "Top %d players by score:" RESET "\n\n", topCount);

		print_player_array_more(&players[n - topCount], topCount);

		free(players);
		return 0;
	}

	if (action == 'R') {
		if (!minScoreSet || !maxScoreSet) {
			print_error(113, NULL, NULL);
			printf("\n");
			print_usage(argv[0]);
			return 1;
		}

		if (minScore > maxScore) {
			print_error(114, NULL, NULL);
			printf("\n");
			return 1;
		}

		if ((players = load_players("build/db/players.csv", &n)) == NULL) {
			return 1;
		}

			quick_sort(players, 0, n - 1, compare_score);

			Player minTarget = {0, "", "", minScore, 0, 0};
		Player maxTarget = {0, "", "", maxScore, 0, 0};
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

	if (action == 'Q') {
		if (!exactScoreSet) {
			print_error(115, NULL, NULL);
			printf("\n");
			print_usage(argv[0]);
			return 1;
		}

		if ((players = load_players("build/db/players.csv", &n)) == NULL) {
			return 1;
		}

			quick_sort(players, 0, n - 1, compare_score);

			Player exactTarget = {0, "", "", exactScore, 0, 0};
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

	if (action == 'd') {
		if (dpOption == DP_INVALID || !budgetSet || budget <= 0) {
			print_error(116, NULL, NULL);
			printf("\n");
			print_usage(argv[0]);
			return 1;
		}

		if ((players = load_players("build/db/players.csv", &n)) == NULL) {
			return 1;
		}

		DPResult dpResult;

		if (dpOption == DP_TABULATION) {
			dpResult = dp_select_team_tabulation(players, n, budget);
			print_dp_result(&dpResult, budget, "tabulacion");
		} else {
			dpResult = dp_select_team_memoization(players, n, budget);
			print_dp_result(&dpResult, budget, "memoizacion");
		}

		free_dp_result(&dpResult);
		free(players);
		return 0;
	}

	if (action == 'e') {
		return run_experiment_menu();
	}

	print_usage(argv[0]);
	return 1;
}

/**
 * @brief Prints the command-line help menu.
 *
 * @param progname Program name used in the usage examples.
 */
static void print_usage(const char *progname){
	printf(LIGHT_BLUE "╔════════════════════════════════════════════════════════════╗\n");
	printf(LIGHT_BLUE "-" WHITE "                       BELURE CLI USAGE                   " LIGHT_BLUE "  -\n");
	printf(LIGHT_BLUE "╚════════════════════════════════════════════════════════════╝\n" RESET);

	printf("\n" DARK_GRAY "Quick use:\n" RESET);
	printf("  " YELLOW "%s " DARK_YELLOW "%-38s" RESET YELLOW "%s\n" RESET,
		progname, "-g <amount> -t <generate type>", "Generate CSV");
	printf("  " YELLOW "%s " ORANGE "%-38s" RESET ORANGE "%s\n" RESET,
		progname, "-r", "Read current CSV");
	printf("  " YELLOW "%s " DARK_BLUE "%-38s" RESET LIGHT_BLUE "%s\n" RESET,
		progname, "-s [-a <sort type>] [-c <criteria>]", "Sort CSV");
	printf("  " YELLOW "%s " DARK_GREEN "%-38s" RESET LIGHT_GREEN "%s\n" RESET,
		progname, "-f [search type] -i <id>", "Search by ID");
	printf("  " YELLOW "%s " MAGENTA "%-38s" RESET MAGENTA "%s\n" RESET,
		progname, "-j <k>", "Show the kth best player");
	printf("  " YELLOW "%s " MAGENTA "%-38s" RESET MAGENTA "%s\n" RESET,
		progname, "-p <N>", "Show top N ranking");
	printf("  " YELLOW "%s " MAGENTA "%-38s" RESET MAGENTA "%s\n" RESET,
		progname, "-q <score>", "Show players with an exact score");
	printf("  " YELLOW "%s " MAGENTA "%-38s" RESET MAGENTA "%s\n" RESET,
		progname, "-R -m <min> -M <max>", "Show players by score range");
	printf("  " YELLOW "%s " LIGHT_GREEN "%-38s" RESET LIGHT_GREEN "%s\n" RESET,
		progname, "-d <method> -b <budget>", "Seleccionar equipo con programacion dinamica");
	printf("  " YELLOW "%s " LIGHT_GREEN "%-38s" RESET LIGHT_GREEN "%s\n" RESET,
		progname, "-G", "Ejecutar estrategias greedy");
	printf("  " YELLOW "%s " PURPLE "%-38s" RESET MAGENTA "%s\n" RESET,
		progname, "-e", "Run experiment");
	printf("  " YELLOW "%s " WHITE "%-38s" RESET WHITE "%s\n" RESET,
		progname, "-h", "Show help");
	printf("\n  " LIGHT_GRAY "Arguments inside [] are optional.\n" RESET);
	printf("  " LIGHT_GRAY "If omitted, the program uses the default algorithm or criterion selected by the authors.\n" RESET);

	printf("\n" DARK_GRAY "Options:\n" RESET);
	printf("  " DARK_YELLOW "%-34s" RESET LIGHT_GRAY "%s\n" RESET,
		"-g, --generate <amount>",
		"Generates a CSV with the given amount");
	printf("  " DARK_YELLOW "%-34s" RESET LIGHT_GRAY "%s\n" RESET,
		"-t, --type <generate type>",
		"Generation type: sorted, inverse, shuffled");
	printf("  " ORANGE "%-34s" RESET LIGHT_GRAY "%s\n" RESET,
		"-r, --read",
		"Reads and prints the current CSV");
	printf("  " DARK_BLUE "%-34s" RESET LIGHT_GRAY "%s\n" RESET,
		"-s, --sort",
		"Sorts the current CSV");
	printf("  " DARK_BLUE "%-34s" RESET LIGHT_GRAY "%s\n" RESET,
		"-a, --algorithm <sort type>",
		"Default: quick. Options: swap, insertion, selection, cocktail, quick, merge, merge-optimized");
	printf("  " DARK_BLUE "%-34s" RESET LIGHT_GRAY "%s\n" RESET,
		"-c, --criteria <criteria>",
		"Default: id. Criteria: id, name, team, score, competitions");
	printf("  " DARK_GREEN "%-34s" RESET LIGHT_GRAY "%s\n" RESET,
		"-f, --find [search type]",
		"Default: interpolation. Options: linear, binary, binary-recursive, exponential, interpolation");
	printf("  " DARK_GREEN "%-34s" RESET LIGHT_GRAY "%s\n" RESET,
		"-i, --id <id>",
		"ID of the player to search for");
	printf("  " MAGENTA "%-34s" RESET LIGHT_GRAY "%s\n" RESET,
		"-j, --kth <k>",
		"Shows the kth best player");
	printf("  " MAGENTA "%-34s" RESET LIGHT_GRAY "%s\n" RESET,
		"-p, --topcount <N>",
		"Shows the ranking of the top N players");
	printf("  " MAGENTA "%-34s" RESET LIGHT_GRAY "%s\n" RESET,
		"-q, --score <score>",
		"Searches players with repeated exact score");
	printf("  " MAGENTA "%-34s" RESET LIGHT_GRAY "%s\n" RESET,
		"-R, --score-range",
		"Enables binary search by score range");
	printf("  " MAGENTA "%-34s" RESET LIGHT_GRAY "%s\n" RESET,
		"-m, --min-score <min>",
		"Minimum score in the range");
	printf("  " MAGENTA "%-34s" RESET LIGHT_GRAY "%s\n" RESET,
		"-M, --max-score <max>",
		"Maximum score in the range");
	printf("  " LIGHT_GREEN "%-34s" RESET LIGHT_GRAY "%s\n" RESET,
		"-d, --dynamic-programming <method>",
		"Method: tabulation, tabulacion, memoization, memoizacion");
	printf("  " LIGHT_GREEN "%-34s" RESET LIGHT_GRAY "%s\n" RESET,
		"-b, --budget <budget>",
		"Maximum budget for team selection");
	printf("  " LIGHT_GREEN "%-34s" RESET LIGHT_GRAY "%s\n" RESET,
		"-G, --greedy",
		"Runs greedy strategies: score, cost, ratio and no budget");
	printf("  " PURPLE "%-34s" RESET LIGHT_GRAY "%s\n" RESET,
		"-e, --experiment",
		"Runs the experiment");
	printf("  " WHITE "%-34s" RESET LIGHT_GRAY "%s\n" RESET,
		"-h, --help",
		"Shows this help");

	printf("\n" DARK_GRAY "Default algorithms:\n" RESET);
	printf("  " LIGHT_GRAY "Some options have a default algorithm assigned.\n" RESET);
	printf("  " LIGHT_GRAY "If no alternative algorithm is specified, the program uses the one chosen by the authors.\n" RESET);
}

/**
 * @brief Parses the requested data generation mode.
 *
 * @param value Generation mode as text.
 * @return GenerationType Parsed generation mode, or `GEN_INVALID`.
 */
static GenerationType parse_generation_type(const char *value){
	if (strcmp(value, "sorted") == 0) return SORTED;
	if (strcmp(value, "inverse") == 0) return INVERSE;
	if (strcmp(value, "shuffled") == 0) return SHUFFLED;
	return GEN_INVALID;
}

/**
 * @brief Parses the requested sorting algorithm.
 *
 * @param value Sorting algorithm as text.
 * @return SortAlgorithm Parsed algorithm, or `SORT_INVALID`.
 */
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

/**
 * @brief Parses the requested sorting criterion.
 *
 * @param value Sorting criterion as text.
 * @return SortCriteria Parsed criterion, or `CRIT_INVALID`.
 */
static SortCriteria parse_sort_criteria(const char *value){
	if (strcmp(value, "id") == 0) return ID;
	if (strcmp(value, "name") == 0) return NAME;
	if (strcmp(value, "team") == 0) return TEAM;
	if (strcmp(value, "score") == 0) return SCORE;
	if (strcmp(value, "competitions") == 0) return COMPETITIONS;
	return CRIT_INVALID;
}

/**
 * @brief Parses the requested search algorithm.
 *
 * @param value Search algorithm as text.
 * @return SearchAlgorithm Parsed algorithm, or `SEARCH_INVALID`.
 */
static SearchAlgorithm parse_search_algorithm(const char *value){
	if (strcmp(value, "linear") == 0) return LINEAR;
	if (strcmp(value, "binary") == 0) return BINARY;
	if (strcmp(value, "binary-recursive") == 0) return BINARY_RECURSIVE;
	if (strcmp(value, "exponential") == 0) return EXPONENTIAL;
	if (strcmp(value, "interpolation") == 0) return INTERPOLATION;
	return SEARCH_INVALID;
}

/**
 * @brief convierte el texto del metodo de programacion dinamica.
 *
 * @param value metodo escrito por consola.
 * @return DPAlgorithm metodo encontrado o `DP_INVALID`.
 */
static DPAlgorithm parse_dp_algorithm(const char *value){
	if (strcmp(value, "tabulation") == 0) return DP_TABULATION;
	if (strcmp(value, "tabulacion") == 0) return DP_TABULATION;
	if (strcmp(value, "memoization") == 0) return DP_MEMOIZATION;
	if (strcmp(value, "memoizacion") == 0) return DP_MEMOIZATION;
	return DP_INVALID;
}

/**
 * @brief muestra el equipo seleccionado por programacion dinamica.
 *
 * @param result resultado de programacion dinamica.
 * @param budget presupuesto ingresado.
 * @param method_name nombre del metodo usado.
 */
static void print_dp_result(DPResult *result, int budget, const char *method_name)
{
	printf(BG_GREEN "Equipo seleccionado con programacion dinamica (%s):" RESET "\n\n", method_name);
	printf(LIGHT_GRAY "Presupuesto: " WHITE "%d\n" RESET, budget);
	printf(LIGHT_GRAY "Costo usado: " WHITE "%d\n" RESET, result->total_cost);
	printf(LIGHT_GRAY "Puntaje total: " WHITE "%.1f\n" RESET, result->total_score);
	printf(LIGHT_GRAY "Deportistas seleccionados: " WHITE "%d\n\n" RESET, result->selected_count);

	if (result->selected_count == 0) {
		printf(BG_RED "No se seleccionaron deportistas con el presupuesto ingresado." RESET "\n\n");
		return;
	}

	print_player_array_more(result->players, result->selected_count);
}

/**
 * @brief Displays the experiment submenu and dispatches the chosen benchmark.
 *
 * @return int Exit status code.
 */
static int run_experiment_menu(void)
{
	int option = 0;

	printf(PURPLE "\n╔════════════════════════════════════╗\n");
	printf(PURPLE "║" WHITE "        MENU DE EXPERIMENTOS       " PURPLE " ║\n");
	printf(PURPLE "╠════════════════════════════════════╣\n");
	printf(PURPLE "║" LIGHT_BLUE " 1. Ordenamiento                    " PURPLE "║\n");
	printf(PURPLE "║" LIGHT_GREEN " 2. Busqueda                        " PURPLE "║\n");
	printf(PURPLE "║" LIGHT_GREEN " 3. PD y Greedy - Tarea 3           " PURPLE "║\n");
	printf(PURPLE "╚════════════════════════════════════╝\n" RESET);
	printf("Selecciona una opcion: ");

	if (scanf("%d", &option) != 1) {
		print_error(116, NULL, NULL);
		return 1;
	}

	if (option == 1) {
		run_sort_experiment();
		return 0;
	}

	if (option == 2) {
		run_search_experiment();
		return 0;
	}

	if (option == 3) {
		return run_team_experiment_menu();
	}

	print_error(116, NULL, NULL);
	return 1;
}
