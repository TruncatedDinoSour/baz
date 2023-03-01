#ifndef _FILE_H
#define _FILE_H
#ifndef FILE_STDDEF
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
static void free_file(File *f) { mem_free(f->content); }

static void alloc_file(const char *path, File *f) {
    int fd;
    size_t content_size;

    if ((fd = open(path, O_RDONLY)) == -1) {
        f->content_size = 0;
        return;
    }

    if ((content_size = lseek(fd, 0, SEEK_END)) == 0) {
        close(fd);
        f->content_size = 0;
        return;
    }

    lseek(fd, 0, SEEK_SET);

    if (content_size >= f->content_size)
        f->content = mem_realloc(f->content, content_size + 2);

    f->content_size = content_size;

    if (read(fd, f->content, content_size) == -1) {
        f->content_size = 0;
        close(fd);
        return;
    }

    f->content[content_size] = '\0';

    close(fd);
}

static char *read_line(const char *path) {
    int fd;
    ssize_t sz = 0, rb;
    char *buf  = NULL;

    if ((fd = open(path, O_RDONLY)) == -1)
        return NULL;

    do {
        buf = mem_realloc(buf, (sz += LINE_GROWTH));

        if ((rb = read(fd, buf, sz)) == -1) {
            mem_free(buf);
            close(fd);
            return NULL;
        }
    } while (rb == sz);

    if (buf[rb - 1] == '\n')
        buf[rb - 1] = '\0';

    return buf;
}
#endif /* FILE_IMPL */
#endif /* _FILE_H */
