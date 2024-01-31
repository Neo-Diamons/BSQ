/*
** EPITECH PROJECT, 2024
** BSQ
** File description:
** bsq
*/

#ifndef BSQ_BSQ_H
    #define BSQ_BSQ_H

    #include <stdbool.h>
    #include <sys/stat.h>
    #include <sys/types.h>

    #define MIN(a, b) ((a) < (b) ? (a) : (b))
    #define MIN3(a, b, c) MIN(MIN((a), (b)), (c))

    #define INLINE inline __attribute__((always_inline))

typedef struct square_s {
    unsigned int off_start;
    unsigned int line_length;

    unsigned int x;
    unsigned int y;
    unsigned int size;
} square_t;

bool bsq(const char *filepath);

char *get_file(const char *filepath, struct stat *st);

int64_t my_atoi(const char *str);

#endif //BSQ_BSQ_H
