/**
 * @file errors.c
 * @author Andres Barbosa, Milton Hernández, Iván Gallardo
 * @brief Funciones para manejo de errores
 */
#include "errors.h"

/**
 * @brief Funcion para imprimir un error
 *
 * @param num Codigo de error
 * @param target Algun parametro que puede ser necesario para el error
 * @param obs Algun texto que puede acompannar el error
 */
void print_error(int num, char *target, char *obs)
{
    if (num >= 100 && num < 200)
    {
        printf(ANSI_COLOR_RED "Error %d: " ANSI_COLOR_RESET, num);
    }
    else if (num >= 200 && num < 300)
    {
        printf(ANSI_COLOR_RED "Fatal Error %d: " ANSI_COLOR_RESET, num);
    }
    else if (num >= 300 && num < 400)
    {
        printf(ANSI_COLOR_YELLOW "Warning %d: " ANSI_COLOR_RESET, num);
    }
    switch (num)
    {
    // Errores generales
    case 100:
        printf("No se pudo leer el archivo %s\n", target);
        break;
    // Errores Fatales
    // Warnings
    // Default
    default:
        printf("Codigo de error desconocido\n");
    }
    if (obs != NULL)
    {
        printf("%s\n", obs);
    }
}