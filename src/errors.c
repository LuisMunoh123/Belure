/**
 * @file errors.c
 * @author Franco Aguilar, Milton Hernández, Luis Muñoz
 * @brief Error handling functions.
 */
#include "errors.h"

/**
 * @brief Prints an error.
 *
 * @param num Error code.
 * @param target Some parameter that may be needed for the error.
 * @param obs Some text that can accompany the error.
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
		printf("File %s could not be read\n", target);
		break;
	case 101:
		printf("File %s could not be opened\n", target);
		break;
	case 102:
		printf("It was not possible to allocate memory for %s\n", target);
		break;
		case 103:
			printf("The number you entered exceeds the maximum value allowed for %s\n", target);
			break;
		case 104:
			printf("You must specify exactly one action\n");
			break;
		case 105:
			printf("To generate data, use -g <amount> and -t <generate type>\n");
			break;
		case 106:
			printf("Invalid sort algorithm\n");
			break;
		case 107:
			printf("Invalid sort criteria\n");
			break;
		case 108:
			printf("Invalid search algorithm\n");
			break;
		case 109:
			printf("To search, use -f [search type] -i <id>\n");
			break;
		case 110:
			printf("Unsupported search algorithm\n");
			break;
		case 111:
			printf("To select, use -j <k>\n");
			break;
		case 112:
			printf("To rank, use -p <N>\n");
			break;
		case 113:
			printf("For score ranges, use -R -m <min> -M <max>\n");
			break;
		case 114:
			printf("The minimum score cannot be greater than the maximum score\n");
			break;
		case 115:
			printf("To search for an exact score, use -q <score>\n");
			break;
		case 116:
			printf("Invalid option\n");
			break;
		// Errores Fatales
		// Warnings
		case 301:
		printf("The number of players is too high, console will show the first %d players\n", MAX_CONSOLE_READABLE_PLAYERS);
		break;
	default:
		printf("unknown error %d\n", num);
	}
	if (obs != NULL)
	{
		printf("%s\n", obs);
	}
}
