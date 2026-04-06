/**
 * @file generator.c
 * @author Andres Barbosa, Milton Hernandez, Ivan Gallardo
 * @brief Funciones de generacion de datos y archivo csv
 */

#include "generator.h"

static char *teams[] = {"Alpha", "Beta", "Gamma", "Delta", "Epsilon", "Theta", "Lambda", "Pi", "Tau"};

/**
 * @brief Funcion para desordenar los jugadores (Fisher-Yates)
 * 
 * @param players Arreglo de jugadores
 * @param n Tamanho del arreglo
 */
static void shuffle_players(Player players[], int n)
{
	for (int i = n - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		swap_player(&players[i], &players[j]);
	}
}

/**
 * @brief Genera una letra aleatoria (Minus y mayus).
 * @return char 
 */
static char random_letter()
{
	char c = 'A' + (rand() % 26);

	if (rand() % 2 == 0)
		c = c + 32;

	return c;
}


/**
 * @brief Genera un jugador con campos ID, nombre, equipo, puntaje y cantidad de competencias.
 * 
 * @param id valor unico de identificacion
 * @param player puntero a la estructura Player donde se almacenaran los datos generados
 */
static void generate_player(int id, Player *player) 
{
	int length = MIN_NAME_LENGTH + (rand() % (MAX_NAME_LENGTH-MIN_NAME_LENGTH));

	player->id = id;

	for (int j = 0; j < length; j++) {
		player->name[j] = random_letter();
	}
	player->name[length] = '\0';

	player->team = teams[rand() % 9];
	player->score = (rand() % 100 + 1) / 10.0f;
	player->competitions = rand() % 251;

	// Aqui no hay nada que ver agente...
	player->potatoe = rand() % 2 ? true : false;
	// Usted no vio nada aqui agente...
}

/**
 * @brief Funcion general de generacion de csv con datos aleatorios 
 * 
 * @param n cantidad de jugadores a generar
 * 
 * @return int 0 si todo va bien, otro codigo de error en caso de error
 */
int generate_csv(int n) 
{
	FILE *csv = fopen("build/db/players.csv", "w");

	if (csv == NULL) {
		print_error(101, "build/db/players.csv", NULL);
		return 101;
	}

	Player *players;

	// Reservamos memoria para los jugadores
	size_t size = n * sizeof(Player);
	char *size_string[10] = sprintf("%.3u GB", size / 1024 / 1024 / 1024);
	if ((players = malloc(n * sizeof(Player))) == NULL) {
		fclose(csv);
		print_error(102, size_string, NULL);
		return 102;
	}
	printf("%s De memoria reservados\n", size_string);
	system("pause");

	if (players == NULL) {
		fclose(csv);
		print_error(102, "malloc", NULL);
		return 102;
	}

	for (int i = 0; i < n; i++) {
		generate_player(i + 1, &players[i]);
	}

	shuffle_players(players, n);

	fprintf(csv, "ID,NAME,TEAM,SCORE,COMPETITIONS,POTATOE\n");

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

	for (int i = 0; i < n; i++) {
		fprintf(csv, "%d,%s,%s,%.1f,%d,%s\n",
			players[i].id,
			players[i].name,
			players[i].team,
			players[i].score,
			players[i].competitions,
			players[i].potatoe ? "true" : "false"
		);

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

	free(players);
	fclose(csv);
	printf("\n" BG_GREEN "Data generated and saved to players.csv" RESET "\n");

	return 0;
}

// Se rie en latex: 𝑗𝑎𝑗𝑎𝑗𝑎𝑗𝑎