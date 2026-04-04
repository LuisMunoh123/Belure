/**
 * @file utilities.h
 * @author Andres Barbosa, Milton Hernández, Iván Gallardo
 * @brief Utilidades micelaneas
 */
#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>

// Colores para texto
#define CLEAR_SCREEN "\033[H\033[J"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define RESET "\x1b[0m"
#define DARK_GRAY "\x1b[90m"
#define LIGHT_GRAY "\x1b[37m"
#define WHITE "\x1b[97m"
#define YELLOW "\x1b[33m"
#define DARK_YELLOW "\x1b[38;5;220m"
#define LIGHT_BLUE "\x1b[34m"
#define DARK_BLUE "\x1b[38;5;20m"
#define LIGHT_GREEN "\x1b[32m"
#define DARK_GREEN "\x1b[38;5;34m"
#define MAGENTA "\x1b[35m"
#define PURPLE "\x1b[95m"
#define LIGHT_RED "\x1b[31m"
#define BG_RED "\x1b[41m"
#define BG_GREEN "\x1b[42m"

#endif