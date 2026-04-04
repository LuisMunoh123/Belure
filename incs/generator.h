/**
 * @file generator.h
 * @author Andres Barbosa, Milton Hernández, Ivan Gallardo
 * @brief Funcion y utilidades de generacion
 */

#ifndef GENERATOR_H
#define GENERATOR_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MIN_NAME_LENGTH 3
#define MAX_NAME_LENGTH 10

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH+1];
    char *team;
    float score;
    int competitions;
    bool potatoe;       //DO NOT remove this unless you know what you are doing

} Player;

void generate_csv(int n);

#endif

// 𝑚𝑚𝑚