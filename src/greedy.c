#include"generator.h"
#include"greedy.h"
#include "player.h"

// compara valores para ordenamiento interno
static int compare_values(const void *a, const void *b)
{
    const Player *p1 = (const Player *)a;
    const Player *p2 = (const Player *)b;

    return p2->value - p1->value;
}

// selecciona los jugadores una vez ordenados
static void select_players(Player players[], Player players_greedy[], int k)
{
    for (int i = 0; i < k; i++) {
        players_greedy[i] = players[i];
    }
}

void greedy_experiment(){

    FILE *csv = fopen("build/db/greedy_players.csv", "w");

    if (csv == NULL) {
		return;
	}

    int n;

    Player *players = load_players("build/db/players.csv", &n);

    if (players == NULL) {
        return;
    }

    // ordenamiento interno
    qsort(players, n, sizeof(Player), compare_values);

    int total_players[] = {10, 20, 30, 40, 50}; // valores de testing
    fprintf(csv, "K,ID,NAME,TEAM,SCORE,COMPETITIONS,VALUE\n");

    for (int i = 0; i < 5; i++) {

        int k = total_players[i];

        if (k > n) {
            continue;
        }

        Player selected_players[k];

        select_players(players, selected_players, k);

        printf("\nTOP %d jugadores:\n", k);

        for (int j = 0; j < k; j++) {

            printf("ID:%d  Nombre:%s  Value:%d\n", selected_players[j].id, selected_players[j].name, selected_players[j].value);
            fprintf(csv,"%d,%d,%s,%s,%.1f,%d,%d\n", k, selected_players[j].id, selected_players[j].name, selected_players[j].team, selected_players[j].score, selected_players[j].competitions, selected_players[j].value);
        }
    }

    free(players);
    fclose(csv);
}