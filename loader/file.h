#ifndef _FILE_H
#define _FILE_H
#ifndef FILE_NO_STDDEF
#include <stddef.h>
#endif

typedef struct {
    size_t content_size;
    char *content;
} File;

static void free_file(File *);
static void alloc_file(const char *, File *);
static char *read_line(const char *);

#ifdef FILE_IMPL
static void free_file(File *f) { free(f->content); }

static void alloc_file(const char *path, File *f) {
    static int fd;
    static size_t content_size;

    if ((fd = open(path, O_RDONLY)) == -1)
        return;

    content_size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    if (content_size > f->content_size) {
        free(f->content);
        f->content = malloc(content_size + 2);
    }

    f->content_size = content_size;

    if (read(fd, f->content, content_size) == -1) {
        close(fd);
        return;
    }

    f->content[content_size] = '\0';
    close(fd);
}

static char *read_line(const char *path) {
    static int fd;
    static ssize_t sz = 0, rb;
    char *buf         = NULL;

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
