#ifndef _PATH_H
#define _PATH_H
#ifndef PATH_NO_DIRENT
#include <dirent.h>
#endif

static unsigned char path_exists(const char *);
static struct dirent *readdir_visible(DIR *);

#ifdef PATH_IMPL
static unsigned char path_exists(const char *path) {
    return access(path, F_OK) == 0;
}

static struct dirent *readdir_visible(DIR *dp) {
    struct dirent *ep;

    while ((ep = readdir(dp)) && *ep->d_name == '.')
        ;

    return ep;
}
#endif /* PATH_IMPL */
#endif /* _PATH_H */
