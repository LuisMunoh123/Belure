#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "dynamic_programming.h"
#include "greedy.h"
#include "player.h"
#include "team_experiment.h"

// Configuracion rapida de los experimentos de Tarea 3.
#define TEAM_DEFAULT_BUDGET 3000
#define TEAM_DEFAULT_K 10
#define TEAM_REPETITIONS 20
#define TEAM_SIZE_COUNT 10
#define TEAM_RESOURCE_LIMIT_SECONDS 5.0
#define SCORE_EPSILON 0.0001f

static const int TEAM_SIZES[TEAM_SIZE_COUNT] = {
    10, 25, 50, 100, 200,
    400, 800, 1200, 1600, 2000
};

static const int RESOURCE_SIZES[] = {
    1000, 2000, 4000, 8000, 12000, 16000
};

#define RESOURCE_SIZE_COUNT ((int)(sizeof(RESOURCE_SIZES) / sizeof(RESOURCE_SIZES[0])))
#define TEAM_COUNTEREXAMPLE_BUDGET 10

typedef enum {
    TEAM_ALGO_GREEDY_SCORE,
    TEAM_ALGO_GREEDY_COST,
    TEAM_ALGO_GREEDY_RATIO,
    TEAM_ALGO_DP_TAB,
    TEAM_ALGO_DP_MEMO,
    TEAM_ALGO_COUNT
} TeamAlgoId;

typedef struct {
    long long total_time_ns;
    double total_cost;
    double total_score;
    double total_selected;
} TeamAccumulator;

static const char *TEAM_ALGO_NAMES[TEAM_ALGO_COUNT] = {
    "greedy_score",
    "greedy_cost",
    "greedy_ratio",
    "dp_tabulation",
    "dp_memoization"
};

static void clear_input_buffer(void)
{
    int c = 0;

    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

static int read_int_with_default(const char *prompt, int default_value)
{
    int value = default_value;

    printf("%s", prompt);

    if (scanf("%d", &value) != 1) {
        clear_input_buffer();
        printf("Valor invalido. Se usa %d.\n", default_value);
        return default_value;
    }

    clear_input_buffer();

    if (value <= 0) {
        printf("Valor invalido. Se usa %d.\n", default_value);
        return default_value;
    }

    return value;
}

static long long now_ns(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + (long long)ts.tv_nsec;
}

static char random_letter(void)
{
    char c = (char)('A' + (rand() % 26));

    if (rand() % 2 == 0) {
        c = (char)(c + 32);
    }

    return c;
}

static void fill_random_name(char name[MAX_NAME_LENGTH + 1])
{
    int length = MIN_NAME_LENGTH + (rand() % (MAX_NAME_LENGTH - MIN_NAME_LENGTH + 1));

    for (int i = 0; i < length; i++) {
        name[i] = random_letter();
    }

    name[length] = '\0';
}

static void generate_random_player(int id, Player *player)
{
    static const char *teams[] = {
        "Alpha", "Beta", "Gamma", "Delta", "Epsilon",
        "Theta", "Lambda", "Pi", "Tau"
    };

    player->id = id;
    fill_random_name(player->name);
    strcpy(player->team, teams[rand() % 9]);
    player->score = (float)((rand() % 100 + 1) / 10.0f);
    player->competitions = rand() % 251;
    player->costo = 100 + rand() % 901;
}

static Player *generate_random_players(int n)
{
    Player *players = NULL;

    if (n <= 0) {
        return NULL;
    }

    players = malloc((size_t)n * sizeof(Player));
    if (players == NULL) {
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        generate_random_player(i + 1, &players[i]);
    }

    return players;
}

static int ensure_output_dir(void)
{
    if (mkdir("build", 0777) != 0 && errno != EEXIST) {
        return 0;
    }

    if (mkdir("build/db", 0777) != 0 && errno != EEXIST) {
        return 0;
    }

    return 1;
}

static float compute_gap_percent(float optimal_score, float algorithm_score)
{
    if (optimal_score <= SCORE_EPSILON) {
        return 0.0f;
    }

    return ((optimal_score - algorithm_score) / optimal_score) * 100.0f;
}

static float compute_quality_percent(float optimal_score, float algorithm_score)
{
    if (optimal_score <= SCORE_EPSILON) {
        if (algorithm_score <= SCORE_EPSILON) {
            return 100.0f;
        }

        return 0.0f;
    }

    return (algorithm_score / optimal_score) * 100.0f;
}

static int is_same_score(float left, float right)
{
    return fabsf(left - right) <= SCORE_EPSILON;
}

static int run_budget_team_experiment(int budget)
{
    FILE *raw_csv = NULL;
    FILE *summary_csv = NULL;

    if (!ensure_output_dir()) {
        printf("No se pudo crear build/db.\n");
        return 1;
    }

    raw_csv = fopen("build/db/team_experiment_raw.csv", "w");
    if (raw_csv == NULL) {
        printf("No se pudo crear build/db/team_experiment_raw.csv\n");
        return 1;
    }

    summary_csv = fopen("build/db/team_experiment_summary.csv", "w");
    if (summary_csv == NULL) {
        fclose(raw_csv);
        printf("No se pudo crear build/db/team_experiment_summary.csv\n");
        return 1;
    }

    fprintf(raw_csv, "n,repeticion,algoritmo,presupuesto,tiempo_ns,costo,puntaje,seleccionados\n");
    fprintf(summary_csv, "n,algoritmo,presupuesto,repeticiones,tiempo_promedio_us,costo_promedio,puntaje_promedio,seleccionados_promedio,brecha_optimo_pct,calidad_pct,coincide_optimo\n");

    for (int i = 0; i < TEAM_SIZE_COUNT; i++) {
        int n = TEAM_SIZES[i];
        TeamAccumulator accumulators[TEAM_ALGO_COUNT] = {0};

        printf("Ejecutando n = %d...\n", n);

        for (int rep = 1; rep <= TEAM_REPETITIONS; rep++) {
            long long start_ns = 0;
            long long elapsed_ns = 0;
            Player *players = generate_random_players(n);

            if (players == NULL) {
                fclose(raw_csv);
                fclose(summary_csv);
                printf("No se pudo reservar memoria para jugadores (n=%d).\n", n);
                return 1;
            }

            start_ns = now_ns();
            GreedyResult greedy_score = greedy_select_team(players, n, budget, GREEDY_SCORE);
            elapsed_ns = now_ns() - start_ns;
            if (greedy_score.players == NULL) {
                free(players);
                fclose(raw_csv);
                fclose(summary_csv);
                printf("Error de memoria en greedy_score (n=%d, rep=%d).\n", n, rep);
                return 1;
            }
            fprintf(raw_csv, "%d,%d,%s,%d,%lld,%d,%.4f,%d\n", n, rep, TEAM_ALGO_NAMES[TEAM_ALGO_GREEDY_SCORE], budget, elapsed_ns, greedy_score.total_cost, greedy_score.total_score, greedy_score.selected_count);
            accumulators[TEAM_ALGO_GREEDY_SCORE].total_time_ns += elapsed_ns;
            accumulators[TEAM_ALGO_GREEDY_SCORE].total_cost += greedy_score.total_cost;
            accumulators[TEAM_ALGO_GREEDY_SCORE].total_score += greedy_score.total_score;
            accumulators[TEAM_ALGO_GREEDY_SCORE].total_selected += greedy_score.selected_count;
            greedy_free_result(&greedy_score);

            start_ns = now_ns();
            GreedyResult greedy_cost = greedy_select_team(players, n, budget, GREEDY_COST);
            elapsed_ns = now_ns() - start_ns;
            if (greedy_cost.players == NULL) {
                free(players);
                fclose(raw_csv);
                fclose(summary_csv);
                printf("Error de memoria en greedy_cost (n=%d, rep=%d).\n", n, rep);
                return 1;
            }
            fprintf(raw_csv, "%d,%d,%s,%d,%lld,%d,%.4f,%d\n", n, rep, TEAM_ALGO_NAMES[TEAM_ALGO_GREEDY_COST], budget, elapsed_ns, greedy_cost.total_cost, greedy_cost.total_score, greedy_cost.selected_count);
            accumulators[TEAM_ALGO_GREEDY_COST].total_time_ns += elapsed_ns;
            accumulators[TEAM_ALGO_GREEDY_COST].total_cost += greedy_cost.total_cost;
            accumulators[TEAM_ALGO_GREEDY_COST].total_score += greedy_cost.total_score;
            accumulators[TEAM_ALGO_GREEDY_COST].total_selected += greedy_cost.selected_count;
            greedy_free_result(&greedy_cost);

            start_ns = now_ns();
            GreedyResult greedy_ratio = greedy_select_team(players, n, budget, GREEDY_RATIO);
            elapsed_ns = now_ns() - start_ns;
            if (greedy_ratio.players == NULL) {
                free(players);
                fclose(raw_csv);
                fclose(summary_csv);
                printf("Error de memoria en greedy_ratio (n=%d, rep=%d).\n", n, rep);
                return 1;
            }
            fprintf(raw_csv, "%d,%d,%s,%d,%lld,%d,%.4f,%d\n", n, rep, TEAM_ALGO_NAMES[TEAM_ALGO_GREEDY_RATIO], budget, elapsed_ns, greedy_ratio.total_cost, greedy_ratio.total_score, greedy_ratio.selected_count);
            accumulators[TEAM_ALGO_GREEDY_RATIO].total_time_ns += elapsed_ns;
            accumulators[TEAM_ALGO_GREEDY_RATIO].total_cost += greedy_ratio.total_cost;
            accumulators[TEAM_ALGO_GREEDY_RATIO].total_score += greedy_ratio.total_score;
            accumulators[TEAM_ALGO_GREEDY_RATIO].total_selected += greedy_ratio.selected_count;
            greedy_free_result(&greedy_ratio);

            start_ns = now_ns();
            DPResult dp_tab = dp_select_team_tabulation(players, n, budget);
            elapsed_ns = now_ns() - start_ns;
            if (dp_tab.players == NULL) {
                free(players);
                fclose(raw_csv);
                fclose(summary_csv);
                printf("Error de memoria en dp_tabulation (n=%d, rep=%d).\n", n, rep);
                return 1;
            }
            fprintf(raw_csv, "%d,%d,%s,%d,%lld,%d,%.4f,%d\n", n, rep, TEAM_ALGO_NAMES[TEAM_ALGO_DP_TAB], budget, elapsed_ns, dp_tab.total_cost, dp_tab.total_score, dp_tab.selected_count);
            accumulators[TEAM_ALGO_DP_TAB].total_time_ns += elapsed_ns;
            accumulators[TEAM_ALGO_DP_TAB].total_cost += dp_tab.total_cost;
            accumulators[TEAM_ALGO_DP_TAB].total_score += dp_tab.total_score;
            accumulators[TEAM_ALGO_DP_TAB].total_selected += dp_tab.selected_count;
            dp_free_result(&dp_tab);

            start_ns = now_ns();
            DPResult dp_memo = dp_select_team_memoization(players, n, budget);
            elapsed_ns = now_ns() - start_ns;
            if (dp_memo.players == NULL) {
                free(players);
                fclose(raw_csv);
                fclose(summary_csv);
                printf("Error de memoria en dp_memoization (n=%d, rep=%d).\n", n, rep);
                return 1;
            }
            fprintf(raw_csv, "%d,%d,%s,%d,%lld,%d,%.4f,%d\n", n, rep, TEAM_ALGO_NAMES[TEAM_ALGO_DP_MEMO], budget, elapsed_ns, dp_memo.total_cost, dp_memo.total_score, dp_memo.selected_count);
            accumulators[TEAM_ALGO_DP_MEMO].total_time_ns += elapsed_ns;
            accumulators[TEAM_ALGO_DP_MEMO].total_cost += dp_memo.total_cost;
            accumulators[TEAM_ALGO_DP_MEMO].total_score += dp_memo.total_score;
            accumulators[TEAM_ALGO_DP_MEMO].total_selected += dp_memo.selected_count;
            dp_free_result(&dp_memo);

            free(players);
        }

        {
            float optimal_score = (float)(accumulators[TEAM_ALGO_DP_TAB].total_score / TEAM_REPETITIONS);

            for (int algo = 0; algo < TEAM_ALGO_COUNT; algo++) {
                float average_time_us = (float)(accumulators[algo].total_time_ns / (double)TEAM_REPETITIONS / 1000.0);
                float average_cost = (float)(accumulators[algo].total_cost / TEAM_REPETITIONS);
                float average_score = (float)(accumulators[algo].total_score / TEAM_REPETITIONS);
                float average_selected = (float)(accumulators[algo].total_selected / TEAM_REPETITIONS);
                float gap = 0.0f;
                float quality = 100.0f;
                int coincide = 0;

                if (algo == TEAM_ALGO_DP_TAB) {
                    gap = 0.0f;
                    quality = 100.0f;
                    coincide = 1;
                } else {
                    gap = compute_gap_percent(optimal_score, average_score);
                    quality = compute_quality_percent(optimal_score, average_score);
                    coincide = is_same_score(optimal_score, average_score) ? 1 : 0;
                }

                fprintf(summary_csv, "%d,%s,%d,%d,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%d\n", n, TEAM_ALGO_NAMES[algo], budget, TEAM_REPETITIONS, average_time_us, average_cost, average_score, average_selected, gap, quality, coincide);
            }
        }
    }

    fclose(raw_csv);
    fclose(summary_csv);

    printf("Resultados guardados correctamente.\n");
    return 0;
}

static int run_no_budget_greedy_experiment(int k)
{
    FILE *summary_csv = NULL;

    if (!ensure_output_dir()) {
        printf("No se pudo crear build/db.\n");
        return 1;
    }

    summary_csv = fopen("build/db/no_budget_greedy_summary.csv", "w");
    if (summary_csv == NULL) {
        printf("No se pudo crear build/db/no_budget_greedy_summary.csv\n");
        return 1;
    }

    fprintf(summary_csv, "n,k,repeticiones,tiempo_promedio_us,puntaje_promedio,seleccionados_promedio\n");

    for (int i = 0; i < TEAM_SIZE_COUNT; i++) {
        int n = TEAM_SIZES[i];
        long long sum_time_ns = 0;
        double sum_score = 0.0;
        double sum_selected = 0.0;

        printf("Ejecutando n = %d...\n", n);

        if (k > n) {
            printf("K=%d no es valido para n=%d. Se omite este tamano.\n", k, n);
            continue;
        }

        for (int rep = 0; rep < TEAM_REPETITIONS; rep++) {
            long long start_ns = 0;
            long long elapsed_ns = 0;
            Player *players = generate_random_players(n);

            if (players == NULL) {
                fclose(summary_csv);
                printf("No se pudo reservar memoria para jugadores (n=%d).\n", n);
                return 1;
            }

            start_ns = now_ns();
            GreedyResult result = greedy_select_top_k(players, n, k);
            elapsed_ns = now_ns() - start_ns;

            if (result.players == NULL) {
                free(players);
                fclose(summary_csv);
                printf("Error de memoria en top_k (n=%d).\n", n);
                return 1;
            }

            sum_time_ns += elapsed_ns;
            sum_score += result.total_score;
            sum_selected += result.selected_count;

            greedy_free_result(&result);
            free(players);
        }

        fprintf(summary_csv, "%d,%d,%d,%.4f,%.4f,%.4f\n", n, k, TEAM_REPETITIONS, sum_time_ns / (double)TEAM_REPETITIONS / 1000.0, sum_score / TEAM_REPETITIONS, sum_selected / TEAM_REPETITIONS);
    }

    fclose(summary_csv);

    printf("Resultados guardados correctamente.\n");
    return 0;
}

static Player build_case_player(int id, const char *name, int costo, float score)
{
    Player player = {0, "", "TeamX", 0.0f, 0, 0};

    player.id = id;
    strncpy(player.name, name, MAX_NAME_LENGTH);
    player.name[MAX_NAME_LENGTH] = '\0';
    player.costo = costo;
    player.score = score;
    player.competitions = 0;

    return player;
}

static int run_greedy_counterexamples(void)
{
    FILE *csv = NULL;

    if (!ensure_output_dir()) {
        printf("No se pudo crear build/db.\n");
        return 1;
    }

    csv = fopen("build/db/greedy_counterexamples.csv", "w");
    if (csv == NULL) {
        printf("No se pudo crear build/db/greedy_counterexamples.csv\n");
        return 1;
    }

    fprintf(csv, "caso,estrategia,presupuesto,puntaje_greedy,puntaje_dp,brecha_optimo_pct,costo_greedy,costo_dp\n");

    {
        Player case_players[] = {
            build_case_player(1, "A", 10, 8.0f),
            build_case_player(2, "B", 6, 5.0f),
            build_case_player(3, "C", 4, 4.0f)
        };
        GreedyResult greedy = greedy_select_team(case_players, 3, TEAM_COUNTEREXAMPLE_BUDGET, GREEDY_SCORE);
        DPResult dp = dp_select_team_tabulation(case_players, 3, TEAM_COUNTEREXAMPLE_BUDGET);
        float gap = compute_gap_percent(dp.total_score, greedy.total_score);

        fprintf(csv, "caso_1,greedy_score,%d,%.4f,%.4f,%.4f,%d,%d\n", TEAM_COUNTEREXAMPLE_BUDGET, greedy.total_score, dp.total_score, gap, greedy.total_cost, dp.total_cost);
        printf("Caso 1: greedy_score=%.1f, dp=%.1f\n", greedy.total_score, dp.total_score);

        greedy_free_result(&greedy);
        dp_free_result(&dp);
    }

    {
        Player case_players[] = {
            build_case_player(1, "A", 2, 0.1f),
            build_case_player(2, "B", 2, 0.1f),
            build_case_player(3, "C", 10, 9.0f)
        };
        GreedyResult greedy = greedy_select_team(case_players, 3, TEAM_COUNTEREXAMPLE_BUDGET, GREEDY_COST);
        DPResult dp = dp_select_team_tabulation(case_players, 3, TEAM_COUNTEREXAMPLE_BUDGET);
        float gap = compute_gap_percent(dp.total_score, greedy.total_score);

        fprintf(csv, "caso_2,greedy_cost,%d,%.4f,%.4f,%.4f,%d,%d\n", TEAM_COUNTEREXAMPLE_BUDGET, greedy.total_score, dp.total_score, gap, greedy.total_cost, dp.total_cost);
        printf("Caso 2: greedy_cost=%.1f, dp=%.1f\n", greedy.total_score, dp.total_score);

        greedy_free_result(&greedy);
        dp_free_result(&dp);
    }

    {
        Player case_players[] = {
            build_case_player(1, "A", 6, 6.0f),
            build_case_player(2, "B", 5, 4.9f),
            build_case_player(3, "C", 5, 4.9f)
        };
        GreedyResult greedy = greedy_select_team(case_players, 3, TEAM_COUNTEREXAMPLE_BUDGET, GREEDY_RATIO);
        DPResult dp = dp_select_team_tabulation(case_players, 3, TEAM_COUNTEREXAMPLE_BUDGET);
        float gap = compute_gap_percent(dp.total_score, greedy.total_score);

        fprintf(csv, "caso_3,greedy_ratio,%d,%.4f,%.4f,%.4f,%d,%d\n", TEAM_COUNTEREXAMPLE_BUDGET, greedy.total_score, dp.total_score, gap, greedy.total_cost, dp.total_cost);
        printf("Caso 3: greedy_ratio=%.1f, dp=%.1f\n", greedy.total_score, dp.total_score);

        greedy_free_result(&greedy);
        dp_free_result(&dp);
    }

    fclose(csv);

    printf("Resultados guardados correctamente.\n");
    return 0;
}

static int run_resource_limit_experiment(void)
{
    FILE *csv = NULL;
    int ratio_active = 1;
    int tab_active = 1;
    int memo_active = 1;

    if (!ensure_output_dir()) {
        printf("No se pudo crear build/db.\n");
        return 1;
    }

    csv = fopen("build/db/team_resource_limits.csv", "w");
    if (csv == NULL) {
        printf("No se pudo crear build/db/team_resource_limits.csv\n");
        return 1;
    }

    fprintf(csv, "n,algoritmo,tiempo_us,completado,dentro_limite\n");

    for (int i = 0; i < RESOURCE_SIZE_COUNT; i++) {
        int n = RESOURCE_SIZES[i];
        Player *players = generate_random_players(n);

        printf("Ejecutando n = %d...\n", n);

        if (players == NULL) {
            fclose(csv);
            printf("No se pudo reservar memoria para n=%d.\n", n);
            return 1;
        }

        if (ratio_active) {
            long long start_ns = now_ns();
            long long elapsed_ns = 0;
            double elapsed_seconds = 0.0;
            int completed = 0;
            int within_limit = 0;
            GreedyResult result = greedy_select_team(players, n, TEAM_DEFAULT_BUDGET, GREEDY_RATIO);

            elapsed_ns = now_ns() - start_ns;
            elapsed_seconds = elapsed_ns / 1000000000.0;
            completed = (result.players != NULL) ? 1 : 0;
            within_limit = (completed && elapsed_seconds <= TEAM_RESOURCE_LIMIT_SECONDS) ? 1 : 0;

            fprintf(csv, "%d,greedy_ratio,%.4f,%d,%d\n", n, elapsed_ns / 1000.0, completed, within_limit);

            if (!completed) {
                printf("greedy_ratio no completo en n=%d.\n", n);
                ratio_active = 0;
            } else if (!within_limit) {
                printf("greedy_ratio supero el limite en n=%d.\n", n);
                ratio_active = 0;
            }

            greedy_free_result(&result);
        }

        if (tab_active) {
            long long start_ns = now_ns();
            long long elapsed_ns = 0;
            double elapsed_seconds = 0.0;
            int completed = 0;
            int within_limit = 0;
            DPResult result = dp_select_team_tabulation(players, n, TEAM_DEFAULT_BUDGET);

            elapsed_ns = now_ns() - start_ns;
            elapsed_seconds = elapsed_ns / 1000000000.0;
            completed = (result.players != NULL) ? 1 : 0;
            within_limit = (completed && elapsed_seconds <= TEAM_RESOURCE_LIMIT_SECONDS) ? 1 : 0;

            fprintf(csv, "%d,dp_tabulation,%.4f,%d,%d\n", n, elapsed_ns / 1000.0, completed, within_limit);

            if (!completed) {
                printf("dp_tabulation no completo en n=%d.\n", n);
                tab_active = 0;
            } else if (!within_limit) {
                printf("dp_tabulation supero el limite en n=%d.\n", n);
                tab_active = 0;
            }

            dp_free_result(&result);
        }

        if (memo_active) {
            long long start_ns = now_ns();
            long long elapsed_ns = 0;
            double elapsed_seconds = 0.0;
            int completed = 0;
            int within_limit = 0;
            DPResult result = dp_select_team_memoization(players, n, TEAM_DEFAULT_BUDGET);

            elapsed_ns = now_ns() - start_ns;
            elapsed_seconds = elapsed_ns / 1000000000.0;
            completed = (result.players != NULL) ? 1 : 0;
            within_limit = (completed && elapsed_seconds <= TEAM_RESOURCE_LIMIT_SECONDS) ? 1 : 0;

            fprintf(csv, "%d,dp_memoization,%.4f,%d,%d\n", n, elapsed_ns / 1000.0, completed, within_limit);

            if (!completed) {
                printf("dp_memoization no completo en n=%d.\n", n);
                memo_active = 0;
            } else if (!within_limit) {
                printf("dp_memoization supero el limite en n=%d.\n", n);
                memo_active = 0;
            }

            dp_free_result(&result);
        }

        free(players);

        if (!ratio_active && !tab_active && !memo_active) {
            break;
        }
    }

    fclose(csv);

    printf("Resultados guardados correctamente.\n");
    return 0;
}

int run_team_experiment_menu(void)
{
    int option = 0;

    while (1) {
        printf("\n===== TAREA 3: PD Y GREEDY =====\n");
        printf("1. Experimento con presupuesto: PD vs Greedy\n");
        printf("2. Experimento Greedy sin presupuesto: Top K\n");
        printf("3. Contraejemplos de estrategias Greedy\n");
        printf("4. Prueba de limites de recursos\n");
        printf("5. Volver\n");
        printf("Selecciona una opcion: ");

        if (scanf("%d", &option) != 1) {
            clear_input_buffer();
            printf("Opcion invalida.\n");
            continue;
        }

        clear_input_buffer();

        if (option == 1) {
            int budget = read_int_with_default("Ingresa presupuesto maximo: ", TEAM_DEFAULT_BUDGET);
            if (run_budget_team_experiment(budget) != 0) {
                return 1;
            }
            continue;
        }

        if (option == 2) {
            int k = read_int_with_default("Ingresa K: ", TEAM_DEFAULT_K);

            if (k > TEAM_SIZES[0]) {
                printf("K debe ser <= %d para cubrir todos los tamanos. Se usa %d.\n", TEAM_SIZES[0], TEAM_DEFAULT_K);
                k = TEAM_DEFAULT_K;
            }

            if (run_no_budget_greedy_experiment(k) != 0) {
                return 1;
            }
            continue;
        }

        if (option == 3) {
            if (run_greedy_counterexamples() != 0) {
                return 1;
            }
            continue;
        }

        if (option == 4) {
            if (run_resource_limit_experiment() != 0) {
                return 1;
            }
            continue;
        }

        if (option == 5) {
            return 0;
        }

        printf("Opcion invalida.\n");
    }
}
