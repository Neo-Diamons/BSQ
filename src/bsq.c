#include <malloc.h>
#include <unistd.h>

#include "bsq.h"

static unsigned int first_line(const char *file, unsigned int *square_map,
    square_t *square, unsigned int *offset)
{
    unsigned int index;

    for (index = *offset;; index++) {
        switch (file[index]) {
            case '.':
                square_map[index - *offset] = 1;
                break;
            case 'o':
                square_map[index - *offset] = 0;
                break;
            case '\n':
                square->line_length = index - *offset + 1;
                return index - (*offset)++;
            default:
                return 0;
        }
    }
}

static bool search(char *file, unsigned int *square_map, square_t *square,
    long long nb_lines)
{
    unsigned int offset = square->offset_start;
    unsigned int line_length = first_line(file, square_map, square, &offset);
    unsigned int count_lines = 1;
    unsigned int index;

    if (!line_length)
        return false;
    for (index = offset + line_length;; index++) {
        switch (file[index]) {
            case '.':
                square_map[index - offset] = file[index - offset - 1] == '\n' ? 1
                    : MIN3(
                        square_map[index - offset - 1],
                        square_map[index - offset - line_length],
                        square_map[index - offset - line_length - 1]
                    ) + 1;
                if (square_map[index - offset] > square->size) {
                    square->x = index;
                    square->y = count_lines;
                    square->size = square_map[index - offset];
                }
                break;
            case 'o':
                square_map[index - offset] = 0;
                break;
            case '\n':
                if (line_length * (++count_lines) != index - offset++)
                    return false;
                break;
            default:
                return !file[index] && file[index - 1] == '\n' && nb_lines == count_lines;
        }
    }
}

static unsigned int get_nb_lines(const char *file, long long *nb_lines)
{
    unsigned int first_line_length = 0;

    if (!file)
        return 0;
    *nb_lines = ice_atoi(file);
    if (nb_lines <= 0)
        return 0;
    for (unsigned long long tmp = *nb_lines; tmp >= 10; first_line_length++)
        tmp /= 10;
    return (first_line_length++ == 0 || file[first_line_length] != '\n') ? 0
        : ++first_line_length;
}

static bool print_square(char *file, struct stat st, square_t *square,
    unsigned int first_line_length)
{
    unsigned int p = (square->y - square->size) * square->line_length
        + (square->x % square->line_length) - square->offset_start + 1;

    for (unsigned int y = 0; y < square->size; ++y) {
        p += square->line_length;
        for (unsigned int x = 0; x < square->size; ++x) {
            file[p + x] = 'x';
        }
    }
    return write(1, file + first_line_length,
        st.st_size - first_line_length) == -1;
}

bool bsq(const char *filepath)
{
    struct stat st;
    char *file = get_file(filepath, &st);
    long long nb_lines;
    unsigned int first_line_length = get_nb_lines(file, &nb_lines);
    unsigned int *square_map = malloc(sizeof(unsigned int) * st.st_size);
    square_t *square = malloc(sizeof(square_t));

    if (!square_map || !square)
        return false;
    *square = (square_t){first_line_length, 0, 0, 0, 0};
    if (!search(file, square_map, square, nb_lines)
        || print_square(file, st, square, first_line_length))
        return false;
    free(file);
    free(square_map);
    free(square);
    return true;
}
