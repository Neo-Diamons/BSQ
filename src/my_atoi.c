/*
** EPITECH PROJECT, 2024
** BSQ
** File description:
** my_atoi
*/

#include <sys/types.h>
#include <stdbool.h>

int64_t my_atoi(const char *str)
{
    bool is_neg = false;
    u_int64_t i = 0;
    int64_t nb = 0;

    for (; str[i] == '+' || str[i] == '-'; i++)
        is_neg = str[i] == '-';
    for (; '0' <= str[i] && str[i] <= '9'; i++)
        nb = nb * 10 + str[i] - '0';
    return is_neg ? -nb : nb;
}
