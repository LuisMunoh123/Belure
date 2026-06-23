#ifndef GREEDY_H
#define GREEDY_H

#include "player.h"

typedef enum {
    GREEDY_SCORE,
    GREEDY_COST,
    GREEDY_RATIO
} GreedyStrategy;

typedef struct {
    Player *players;
    int selected_count;
    int total_cost;
    float total_score;
} GreedyResult;

#define GREEDY_DEFAULT_BUDGET 3000

GreedyResult greedy_select_team(const Player *players, int n, int budget, GreedyStrategy strategy);
GreedyResult greedy_select_top_k(const Player *players, int n, int k);
void greedy_experiment(void);
void greedy_free_result(GreedyResult *result);
void free_greedy_result(GreedyResult *result);

#endif
