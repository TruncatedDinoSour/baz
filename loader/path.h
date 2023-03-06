#ifndef _PATH_H
#define _PATH_H
#ifndef PATH_NO_LIBS
#include <dirent.h>
#endif

static char *get_base(const char *);
static unsigned char path_exists(const char *);
static struct dirent *readdir_visible(DIR *);

#ifdef PATH_IMPL
static char *get_base(const char *path) {
    char *last_slash = strrchr(path, '/');
    char *buf        = malloc(strlen(last_slash));

    memmove(buf, last_slash + 1, strlen(last_slash));
    return buf;
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
