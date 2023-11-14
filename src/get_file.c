#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

char *get_file(const char *filepath, struct stat *st) {
    char *buffer;
    int fd;

    if (stat(filepath, st) == -1 || !S_ISREG(st->st_mode))
        return NULL;
    buffer = malloc(st->st_size + 1);
    if (!buffer)
        return NULL;
    fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        free(buffer);
        return NULL;
    }
    if (read(fd, buffer, st->st_size) != st->st_size || close(fd) == -1) {
        free(buffer);
        return NULL;
    }
    buffer[st->st_size] = '\0';
    return buffer;
}
