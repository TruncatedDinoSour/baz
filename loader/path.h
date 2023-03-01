#ifndef _PATH_H
#define _PATH_H
#ifndef PATH_NO_LIBS
#include <sys/stat.h>
#include <dirent.h>
#endif

static char *chop_str(const char *, const unsigned long, const unsigned long);
static char *get_base(const char *);
static unsigned char path_exists(const char *);
static struct dirent *readdir_visible(DIR *);

#ifdef PATH_IMPL
static char *
chop_str(const char *str, const unsigned long begin, const unsigned long end) {
    char *buf;
    unsigned long idx;

    if (begin >= end)
        return NULL;

    buf = mem_alloc((end - begin + 1) * sizeof(char));

    for (idx = begin; idx <= end; ++idx)
        buf[idx - begin] = str[idx];

    buf[idx] = '\0';

    return buf;
}

static char *get_base(const char *path) {
    const size_t len         = strlen(path);
    unsigned long last_slash = 0, idx;

    for (idx = 0; idx < len; ++idx)
        if (path[idx] == '/')
            last_slash = idx;

    return chop_str(path, last_slash + 1, len);
}

static unsigned char path_exists(const char *path) {
    return access(path, F_OK) == 0;
}

static struct dirent *readdir_visible(DIR *dp) {
    struct dirent *ep;

    do {
        ep = readdir(dp);
    } while (ep && ep->d_name[0] == '.');

    return ep;
}
#endif /* PATH_IMPL */
#endif /* _PATH_H */
