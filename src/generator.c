/**
 * @file generator.c
 * @author Andres Barbosa, Milton Hernandez, Ivan Gallardo, Franco Aguilar
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
 * @brief Funcion para invertir el orden de los jugadores
 * 
 * @param players Arreglo de jugadores a invertir
 * @param n Tamanho del arreglo
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
 * @brief Funcion general de generacion de csv con datos aleatorios 
 * 
 * @param n cantidad de jugadores a generar
 * 
 * @return int 0 si todo va bien, otro codigo de error en caso de error
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
	printf(BG_GREEN "%s De memoria reservados" RESET"\n", size_string);

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

	print_player_array_more(players, n);

	free(players);
	fclose(csv);
	printf("\n" BG_GREEN "Data generated and saved to build/db/players.csv" RESET "\n");

	return 0;
}

/**
 * @brief Funcion para cargar datos de jugadores de un archivo csv
 * 
 * @param file Direccion relativa al archivo csv
 * @param out_n Puntero a la variable que almacenara el numero de jugadores
 * @return Player* Puntero al arreglo de jugadores
 * @important El arreglo debe ser liberado con free()
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
		print_error(103, "No se pudo leer la cantidad de jugadores", NULL);
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

	printf("%s de memoria reservados\n", size_string);

	// Leer cabecera
	if (fscanf(csv, "%*s %*s %*s %*s %*s") == EOF) {
		free(playerArray);
		fclose(csv);
		print_error(101, file, "No se pudo leer la cabecera");
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
			print_error(101, file, "CSV malformada");
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