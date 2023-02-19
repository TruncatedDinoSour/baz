#ifndef _FILE_H
#define _FILE_H
#ifndef FILE_NO_STDDEF
#include <stddef.h>
#endif

typedef struct {
    size_t content_size;
    char *content;
} File;

File read_file(const char *);
void close_file(File *);

#ifdef FILE_IMPL
File read_file(const char *path) {
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

    f.content = malloc(content_size + 2);

    if (read(fd, f.content, content_size) == -1) {
        free(f.content);
        f.content = NULL;
        return f;
    }

    f.content[content_size] = '\0';

    f.content_size = content_size;

    close(fd);

    return f;
}

void close_file(File *f) { free(f->content); }
#endif /* FILE_IMPL */
#endif /* _FILE_H */
