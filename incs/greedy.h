#ifndef GREEDY_H
#define GREEDY_H

#include "player.h"

typedef enum {
    GREEDY_SCORE,
    GREEDY_COST,
    GREEDY_RATIO,
    GREEDY_NO_BUDGET
} GreedyStrategy;

typedef struct {
    Player *players;
    int selected_count;
    int total_cost;
    int total_score;
} GreedyResult;

GreedyResult greedy_select_team(Player players[], int n, int budget, GreedyStrategy strategy);
void greedy_experiment(void);
void free_greedy_result(GreedyResult *result);

#endif
