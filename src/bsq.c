/*
** EPITECH PROJECT, 2024
** BSQ
** File description:
** bsq
*/

#include <malloc.h>
#include <unistd.h>

#include "bsq.h"

INLINE static void update_square(
    square_t *square,
    uint x,
    uint y,
    uint size)
{
    square->x = x;
    square->y = y;
    square->size = size;
}

INLINE static uint first_line(
    const char *file,
    uint *map,
    square_t *square,
    uint *off)
{
    for (uint idx = *off;; idx++) {
        switch (file[idx]) {
            case '.':
                map[idx - *off] = 1;
                if (map[idx - *off] > square->size) {
                    square->x = idx;
                    square->y = 0;
                    square->size = map[idx - *off];
                }
                break;
            case 'o':
                map[idx - *off] = 0;
                break;
            case '\n':
                square->line_length = idx - *off + 1;
                return idx - (*off)++;
            default:
                return 0;
        }
    }
}

INLINE static bool main_loop(
    const char *file,
    uint *map,
    square_t *square,
    uint off,
    uint line_length,
    int64_t nb_lines)
{
    uint count_lines = 1;
    for (uint idx = off + line_length + 1;; idx++) {
        switch (file[idx]) {
            case '.':
                map[idx - off] = file[idx - 1] == '\n' ? 1
                    : MIN3(
                        map[idx - off - 1],
                        map[idx - off - line_length],
                        map[idx - off - line_length - 1]
                    ) + 1;
                if (map[idx - off] > square->size)
                    update_square(square, idx, count_lines, map[idx - off]);
                break;
            case 'o':
                map[idx - off] = 0;
                break;
            case '\n':
                if (line_length * (++count_lines) != idx - off++)
                    return false;
                break;
            default:
                return !file[idx] && file[idx - 1] == '\n'
                    && nb_lines == count_lines;
        }
    }
}

INLINE static bool search(
    char *file,
    uint *map,
    square_t *square,
    int64_t nb_lines)
{
    uint off = square->off_start;
    uint line_length = first_line(file, map, square, &off);

    if (!line_length)
        return false;
    switch (file[off + line_length]) {
        case '.':
            map[line_length] = 1;
            if (map[line_length] > square->size)
                update_square(square, off + line_length, 1, map[line_length]);
            break;
        case 'o':
            map[line_length] = 0;
            break;
        default:
            return nb_lines == 1;
    }
    return main_loop(file, map, square, off, line_length, nb_lines);
}

INLINE static uint get_nb_lines(
    const char *file,
    int64_t *nb_lines)
{
    uint first_line_length = 0;

    if (!file)
        return 0;
    *nb_lines = my_atoi(file);
    if (*nb_lines <= 0)
        return 0;
    for (uint tmp = *nb_lines; tmp >= 10; first_line_length++)
        tmp /= 10;
    first_line_length++;
    return *nb_lines == 0 || file[first_line_length] != '\n' ? 0
        : ++first_line_length;
}

INLINE static bool print_square(
    char *file,
    struct stat st,
    square_t *square,
    uint first_line_length)
{
    if (square->line_length == 2) {
        for (uint i = 0; i < square->size; ++i)
            file[square->x + i] = 'x';
    } else {
        uint p = (square->y - square->size) * square->line_length
            + (square->x - square->size + 1) % square->line_length;
        for (uint y = 0; y < square->size; ++y) {
            p += square->line_length;
            for (uint x = 0; x < square->size; ++x)
                file[p + x] = 'x';
        }
    }

    return write(STDOUT_FILENO, file + first_line_length,
        st.st_size - first_line_length) == EOF;
}

INLINE static bool find_square(struct stat st, char *file)
{
    bool ret = true;
    int64_t nb_lines;
    uint first_line_length = get_nb_lines(file, &nb_lines);
    square_t square = {first_line_length, 0, 0, 0, 0};
    uint *map = malloc(sizeof(uint) * st.st_size);

    if (!map) {
        free(file);
        return false;
    }
    if (!search(file, map, &square, nb_lines)
        || print_square(file, st, &square, first_line_length))
        ret = false;
    free(file);
    free(map);
    return ret;
}

bool bsq(const char *filepath)
{
    struct stat st = {0};
    char *file = get_file(filepath, &st);

    return file ? find_square(st, file) : false;
}
