#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "greedy.h"
#include "generator.h"
#include "sorting.h"

static int compare_greedy_score(const void *a, const void *b)
{
    const Player *p1 = (const Player *)a;
    const Player *p2 = (const Player *)b;

    if (p2->score > p1->score) return 1;
    if (p2->score < p1->score) return -1;
    return 0;
}

static int compare_greedy_cost(const void *a, const void *b)
{
    const Player *p1 = (const Player *)a;
    const Player *p2 = (const Player *)b;

    return p1->costo - p2->costo;
}

static int compare_greedy_ratio(const void *a, const void *b)
{
    const Player *p1 = (const Player *)a;
    const Player *p2 = (const Player *)b;

    float ratio1 = 0.0f;
    float ratio2 = 0.0f;

    if (p1->costo > 0) {
        ratio1 = p1->score / (float)p1->costo;
    }

    if (p2->costo > 0) {
        ratio2 = p2->score / (float)p2->costo;
    }

    if (ratio2 > ratio1) return 1;
    if (ratio2 < ratio1) return -1;
    return 0;
}

static GreedyResult empty_greedy_result(void)
{
    GreedyResult result;
    result.players = NULL;
    result.selected_count = 0;
    result.total_cost = 0;
    result.total_score = 0.0f;
    return result;
}

static int is_valid_budget_strategy(GreedyStrategy strategy)
{
    return strategy == GREEDY_SCORE || strategy == GREEDY_COST || strategy == GREEDY_RATIO;
}

static void sort_players_for_strategy(Player *players, int n, GreedyStrategy strategy)
{
    if (strategy == GREEDY_SCORE) {
        qsort(players, n, sizeof(Player), compare_greedy_score);
    } else if (strategy == GREEDY_COST) {
        qsort(players, n, sizeof(Player), compare_greedy_cost);
    } else if (strategy == GREEDY_RATIO) {
        qsort(players, n, sizeof(Player), compare_greedy_ratio);
    }
}

GreedyResult greedy_select_team(const Player *players, int n, int budget, GreedyStrategy strategy)
{
    if (players == NULL || n <= 0 || budget <= 0 || !is_valid_budget_strategy(strategy)) {
        return empty_greedy_result();
    }

    Player *copy = malloc(n * sizeof(Player));
    Player *selected = malloc(n * sizeof(Player));

    if (copy == NULL || selected == NULL) {
        free(copy);
        free(selected);
        return empty_greedy_result();
    }

    memcpy(copy, players, n * sizeof(Player));
    sort_players_for_strategy(copy, n, strategy);

    GreedyResult result = empty_greedy_result();

    for (int i = 0; i < n; i++) {
        if (result.total_cost + copy[i].costo <= budget) {
            selected[result.selected_count] = copy[i];
            result.selected_count++;
            result.total_cost += copy[i].costo;
            result.total_score += copy[i].score;
        }
    }

    result.players = selected;

    free(copy);
    return result;
}

GreedyResult greedy_select_top_k(const Player *players, int n, int k)
{
    if (players == NULL || n <= 0 || k <= 0 || k > n) {
        return empty_greedy_result();
    }

    Player *copy = malloc(n * sizeof(Player));
    Player *selected = malloc(k * sizeof(Player));

    if (copy == NULL || selected == NULL) {
        free(copy);
        free(selected);
        return empty_greedy_result();
    }

    memcpy(copy, players, n * sizeof(Player));
    qsort(copy, n, sizeof(Player), compare_greedy_score);

    GreedyResult result = empty_greedy_result();
    result.players = selected;

    for (int i = 0; i < k; i++) {
        result.players[i] = copy[i];
        result.selected_count++;
        result.total_cost += copy[i].costo;
        result.total_score += copy[i].score;
    }

    free(copy);
    return result;
}

void greedy_free_result(GreedyResult *result)
{
    if (result == NULL) {
        return;
    }

    free(result->players);
    result->players = NULL;
    result->selected_count = 0;
    result->total_cost = 0;
    result->total_score = 0.0f;
}

void free_greedy_result(GreedyResult *result)
{
    greedy_free_result(result);
}

static void print_greedy_result(GreedyResult *result, const char *name, int budget)
{
    printf("\nResultado greedy: %s\n", name);

    if (budget > 0) {
        printf("Presupuesto: %d\n", budget);
    } else {
        printf("Sin restriccion de presupuesto\n");
    }

    printf("Costo usado: %d\n", result->total_cost);
    printf("Puntaje total: %.1f\n", result->total_score);
    printf("Deportistas seleccionados: %d\n\n", result->selected_count);

    print_player_array_more(result->players, result->selected_count);
}

void greedy_experiment(void)
{
    int n;
    int budget = GREEDY_DEFAULT_BUDGET;

    Player *players = load_players("build/db/players.csv", &n);

    if (players == NULL) {
        printf("No se pudo cargar build/db/players.csv\n");
        return;
    }

    GreedyResult by_score = greedy_select_team(players, n, budget, GREEDY_SCORE);
    GreedyResult by_cost = greedy_select_team(players, n, budget, GREEDY_COST);
    GreedyResult by_ratio = greedy_select_team(players, n, budget, GREEDY_RATIO);
    GreedyResult no_budget = greedy_select_top_k(players, n, n);

    print_greedy_result(&by_score, "mayor score", budget);
    print_greedy_result(&by_cost, "menor costo", budget);
    print_greedy_result(&by_ratio, "mejor relacion score/costo", budget);
    print_greedy_result(&no_budget, "sin presupuesto (top n)", 0);

    FILE *csv = fopen("build/db/greedy_results.csv", "w");

    if (csv != NULL) {
        fprintf(csv, "strategy,selected_count,total_cost,total_score\n");
        fprintf(csv, "score,%d,%d,%.1f\n", by_score.selected_count, by_score.total_cost, by_score.total_score);
        fprintf(csv, "cost,%d,%d,%.1f\n", by_cost.selected_count, by_cost.total_cost, by_cost.total_score);
        fprintf(csv, "ratio,%d,%d,%.1f\n", by_ratio.selected_count, by_ratio.total_cost, by_ratio.total_score);
        fprintf(csv, "no_budget,%d,%d,%.1f\n", no_budget.selected_count, no_budget.total_cost, no_budget.total_score);
        fclose(csv);
    }

    greedy_free_result(&by_score);
    greedy_free_result(&by_cost);
    greedy_free_result(&by_ratio);
    greedy_free_result(&no_budget);
    free(players);
}
