/**
 * @file generator.c
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz, Franco Aguilar
 * @brief Data generation and CSV file utilities.
 */

#include "generator.h"

static char *teams[] = {"Alpha", "Beta", "Gamma", "Delta", "Epsilon", "Theta", "Lambda", "Pi", "Tau"};

/**
 * @brief Shuffles the players (Fisher-Yates).
 * 
 * @param players Player array.
 * @param n Array size.
 */
static void shuffle_players(Player players[], int n)
{
	for (int i = n - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		swap_player(&players[i], &players[j]);
	}
}

/**
 * @brief Reverses the order of the players.
 * 
 * @param players Player array to reverse.
 * @param n Array size.
 */
static void reverse_players(Player players[], int n)
{
	int left = 0;
	int right = n - 1;

	while (left < right) {
		swap_player(&players[left], &players[right]);
		left++;
		right--;
	}
}

/**
 * @brief Generates a random letter (uppercase or lowercase).
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
 * @brief Generates a player with ID, name, team, score, and number of competitions.
 * 
 * @param id Unique identifier value.
 * @param player Pointer to the Player structure where generated data will be stored.
 */
static void generate_player(int id, Player *player) 
{
	int length = MIN_NAME_LENGTH + (rand() % (MAX_NAME_LENGTH - MIN_NAME_LENGTH + 1));

	player->id = id;

	for (int j = 0; j < length; j++) {
		player->name[j] = random_letter();
	}
	player->name[length] = '\0';

	strcpy(player->team, teams[rand() % 9]);
	player->score = (rand() % 100 + 1) / 10.0f;
	player->competitions = rand() % 251;
}

/**
 * @brief General CSV generation function with random data.
 * 
 * @param n Number of players to generate.
 * 
 * @return int 0 if everything goes well, another error code otherwise.
 */
int generate_csv(int n, GenerationType generationType) 
{
	FILE *csv = fopen("build/db/players.csv", "w");

	if (csv == NULL) {
		print_error(101, "build/db/players.csv", NULL);
		return 101;
	}

	Player *players;

	// Reservamos memoria para los jugadores
	size_t size = n * sizeof(Player);
	char size_string[32];
	sprintf(size_string, "%lu MB", (unsigned long)(size / 1024 / 1024));

	if ((players = malloc(n * sizeof(Player))) == NULL) {
		fclose(csv);
		print_error(102, size_string, NULL);
		return 102;
	}
	for (int i = 0; i < n; i++) {
		generate_player(i + 1, &players[i]);
	}

	if (generationType == INVERSE) {
		reverse_players(players, n);
	} else if (generationType == SHUFFLED) {
		shuffle_players(players, n);
	}

	// Imprimimos cabecera en el archivo csv
	fprintf(csv, "%d\n", n);
	fprintf(csv, "ID NAME TEAM SCORE COMPETITIONS\n");

	// Imprimimos los datos en el archivo csv
	for (int i = 0; i < n; i++) {
		fprintf(csv, "%d %s %s %.1f %d\n",
			players[i].id,
			players[i].name,
			players[i].team,
			players[i].score,
			players[i].competitions
		);
	}

	free(players);
	fclose(csv);
	printf("\nData generated and saved to build/db/players.csv" RESET "\n");

	return 0;
}

/**
 * @brief Loads player data from a CSV file.
 * 
 * @param file Relative path to the CSV file.
 * @param out_n Pointer to the variable that stores the number of players.
 * @return Player* Pointer to the player array.
 * @important The array must be released with free().
 */
Player* load_players(char* file, int* out_n)
{
	FILE *csv = fopen(file, "r");
	if (csv == NULL) {
		print_error(101, file, NULL);
		return NULL;
	}

	int n;
	Player* playerArray = NULL;

	// Leer cantidad de jugadores
	if (fscanf(csv, "%d", &n) != 1 || n <= 0) {
		fclose(csv);
		print_error(103, "Could not read the number of players", NULL);
		return NULL;
	}

	// Reservar memoria
	size_t size = (size_t)n * sizeof(Player);
	char size_string[32];
	sprintf(size_string, "%lu MB", (unsigned long)(size / 1024 / 1024));

	playerArray = malloc(size);
	if (playerArray == NULL) {
		fclose(csv);
		print_error(102,  size_string, NULL);
		return NULL;
	}

	// Leer cabecera
	if (fscanf(csv, "%*s %*s %*s %*s %*s") == EOF) {
		free(playerArray);
		fclose(csv);
		print_error(101, file, "Could not read the header");
		return NULL;
	}

	// Leer jugadores
	for (int i = 0; i < n; i++) {
		// Leemos los datos de la linea
		int fields = fscanf(csv, "%d %10s %10s %f %d",
			&playerArray[i].id,
			playerArray[i].name,
			playerArray[i].team,
			&playerArray[i].score,
			&playerArray[i].competitions
		);
	
		if (fields != 5) {
			free(playerArray);
			fclose(csv);
			print_error(101, file, "Malformed CSV");
			return NULL;
		}
	}

	// Enviamos la cantidad de jugadores si el puntero fue pasado
	if (out_n != NULL) {
		*out_n = n;
	}
	fclose(csv);
	return playerArray;
}
