#ifndef _PATH_H
#define _PATH_H
#include <dirent.h>
#include <unistd.h>

static unsigned char path_exists(const char *const);
static struct dirent *readdir_visible(DIR *const);

#ifdef PATH_IMPL
static unsigned char path_exists(const char *const path) {
    return access(path, F_OK) == 0;
}

static struct dirent *readdir_visible(DIR *const dp) {
    static struct dirent *ep;

    while ((ep = readdir(dp)) && *ep->d_name == '.')
        ;

    return ep;
}
#endif /* PATH_IMPL */
#endif /* _PATH_H */
