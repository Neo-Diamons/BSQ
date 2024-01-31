/*
** EPITECH PROJECT, 2024
** BSQ
** File description:
** main
*/

#include <stdlib.h>

#include "bsq.h"

int main(int ac, char **av)
{
    if (ac != 2)
        return EXIT_FAILURE;
    return bsq(av[1]) ? EXIT_SUCCESS : EXIT_FAILURE;
}
