#ifndef _FILE_H
#define _FILE_H
#include "config.h"

#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

typedef struct {
    size_t content_size;
    char *content;
} File;

static void free_file(const File *const);
static void alloc_file(const char *const, File *const);
static char *read_line(const char *const);

#ifdef FILE_IMPL
static void free_file(const File *const f) { free(f->content); }

static void alloc_file(const char *const path, File *const f) {
    static int fd;
    static size_t content_size;

    if ((fd = open(path, O_RDONLY)) == -1)
        return;

    content_size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    if (content_size > f->content_size) {
        free(f->content);
        f->content = malloc(content_size + 1);
    }

    f->content_size = content_size;

    if (read(fd, f->content, content_size) == -1) {
        close(fd);
        return;
    }

    f->content[content_size] = '\0';
    close(fd);
}

static char *read_line(const char *const path) {
    static int fd;
    static ssize_t sz, rb;
    static char *buf;

    sz  = 0;
    buf = 0;

    if ((fd = open(path, O_RDONLY)) == -1)
        return NULL;

    do {
        buf = realloc(buf, (sz += LINE_GROWTH));

        if ((rb = read(fd, buf, sz)) == -1) {
            free(buf);
            close(fd);
            return NULL;
        }
    } while (rb == sz);

    buf[rb - (buf[rb - 1] == '\n')] = '\0';
    close(fd);

    return buf;
}
#endif /* FILE_IMPL */
#endif /* _FILE_H */
