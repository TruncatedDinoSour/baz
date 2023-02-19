#ifndef _PATH_H
#define _PATH_H
#ifndef PATH_NO_LIBS
#include <sys/stat.h>
#include <dirent.h>
#endif

char *chop_str(const char *, const unsigned long, const unsigned long);
char *get_base(const char *);
unsigned char path_exists(const char *);
struct dirent *readdir_visible(DIR *);

#ifdef PATH_IMPL
char *
chop_str(const char *str, const unsigned long begin, const unsigned long end) {
    char *buf;
    unsigned long idx;

    if (begin >= end)
        return NULL;

    buf = malloc((sizeof str[0]) * (end - begin) + 1);

    for (idx = begin; idx <= end; ++idx)
        buf[idx - begin] = str[idx];

    buf[idx] = '\0';

    return buf;
}

char *get_base(const char *path) {
    const size_t len         = strlen(path);
    unsigned long last_slash = 0, idx;

    for (idx = 0; idx < len; ++idx) {
        if (path[idx] == '/')
            last_slash = idx;
    }

    return chop_str(path, last_slash + 1, len);
}

unsigned char path_exists(const char *path) {
    struct stat buffer;
    return stat(path, &buffer) == 0;
}

struct dirent *readdir_visible(DIR *dp) {
    struct dirent *ep;

    while ((ep = readdir(dp)))
        if (ep->d_name[0] != '.')
            break;

    return ep;
}
#endif /* PATH_IMPL */
#endif /* _PATH_H */
