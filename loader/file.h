#ifndef _FILE_H
#define _FILE_H
#ifndef FILE_STDDEF
#include <stddef.h>
#endif

typedef struct {
    size_t content_size;
    char *content;
} File;

static void close_file(File *);
static File read_file(const char *);
static char *read_line(const char *);

#ifdef FILE_IMPL
static void close_file(File *f) { mem_free(f->content); }

static File read_file(const char *path) {
    File f = {0};
    size_t content_size;

    int fd = open(path, O_RDONLY);

    if (fd == -1)
        return f;

    content_size = lseek(fd, 0, SEEK_END);

    if (content_size == 0) {
        close(fd);
        return f;
    }

    lseek(fd, 0, SEEK_SET);

    f.content = mem_alloc(content_size + 2);

    if (read(fd, f.content, content_size) == -1) {
        close_file(&f);
        f.content = NULL;
        return f;
    }

    f.content[content_size] = '\0';

    f.content_size = content_size;

    close(fd);

    return f;
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
