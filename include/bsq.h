#ifndef BSQ_BSQ_H
    #define BSQ_BSQ_H

    #include <stdbool.h>
    #include <sys/stat.h>

    #include "types.h"

    #define MIN(a, b) ((a) < (b) ? (a) : (b))
    #define MIN3(a, b, c) MIN(MIN((a), (b)), (c))

typedef struct {
    unsigned int offset_start;
    unsigned int line_length;

    unsigned int x;
    unsigned int y;
    unsigned int size;
} square_t;

bool bsq(const char *filepath);

char *get_file(const char *filepath, struct stat *st);

ll_t ice_atoi(const char *str);

#endif //BSQ_BSQ_H
