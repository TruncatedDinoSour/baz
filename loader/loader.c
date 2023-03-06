#include "config.h"

#define FILE_NO_STDDEF
#define STR_NO_STDDEF
#define PATH_NO_LIBS

#define FILE_IMPL
#define PATH_IMPL
#define STR_IMPL
#define SHELL_IMPL

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>

#ifdef ALLOW_ALLOCA
#include <alloca.h>
#endif

#include "file.h"
#include "macros.h"
#include "path.h"
#include "shell.h"
#include "str.h"

#ifdef LOGGING
static unsigned char debug_load;
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

typedef void (*stage_t)(char *);

static void load_envs(char *path) {
    static volatile size_t envs_base;

    struct dirent *ep;
    DIR *dp;

    mks(File, f);
    mks(Str, s);

    log("loading environment variables");

    strcat(path, DP(ENVS_DIR));
    envs_base = strlen(path);

    if ((dp = opendir(path)) == NULL)
        return;

    nputs("export");

    while ((ep = readdir_visible(dp))) {
        strcat(path, ep->d_name);

        alloc_file(path, pf);

        if (f.content_size == 0)
            continue;

        log(path);

        if (f.content[f.content_size - 1] == '\n')
            f.content[f.content_size - 1] = '\0';

        escape_quotes(ps, f.content, f.content_size);

        printf(" %s=\"%s\"", ep->d_name, s.string);
        path[envs_base] = '\0';
    }

    pnl();
    free_file(pf);
    str_free(ps);
    closedir(dp);
}

static void load_cmds(char *path) {
    static volatile size_t cmds_base;

    struct dirent *ep;
    DIR *dp;

    log("adding commands");

    strcat(path, DP(CMDS_DIR));
    cmds_base = strlen(path);

    if ((dp = opendir(path)) == NULL)
        return;

    printf("export PATH=\"%s:$PATH\"\n", path);

    while ((ep = readdir_visible(dp))) {
        strcat(path, ep->d_name);
        chmod(path, 0700);
        path[cmds_base] = '\0';
    }
}

static void load_functions(char *path) {
    static volatile size_t funs_base;

    struct dirent *ep;
    DIR *dp;

    mks(File, f);

    log("loading functions");

    strcat(path, DP(FUNCS_DIR));
    funs_base = strlen(path);

    if ((dp = opendir(path)) == NULL)
        return;

    while ((ep = readdir_visible(dp))) {
        strcat(path, ep->d_name);

        alloc_file(path, pf);

        if (f.content_size == 0)
            continue;

        log(path);
        printf("%s(){\n%s\n}\n", ep->d_name, f.content);

        path[funs_base] = '\0';
    }

    free_file(pf);
    closedir(dp);
}

static void load_aliases(char *path) {
    static volatile size_t alias_base;

    struct dirent *ep;
    DIR *dp;

    mks(Str, s);
    mks(File, f);

    log("loading aliases");

    strcat(path, DP(ALIASES_DIR));

    alias_base = strlen(path);

    if ((dp = opendir(path)) == NULL)
        return;

    nputs("alias");

    while ((ep = readdir_visible(dp))) {
        strcat(path, ep->d_name);

        log(path);

        alloc_file(path, pf);
        escape_quotes(ps, f.content, f.content_size);

        printf(" %s=\"%s\"", ep->d_name, s.string);
        path[alias_base] = '\0';
    }

    pnl();
    free_file(pf);
    str_free(ps);
    closedir(dp);
}

static void load_runners(char *path) {
    static volatile size_t runs_base;

    struct dirent *ep;
    DIR *dp;

    mks(File, f);

    log("running runners");

    strcat(path, DP(RUNS_DIR));
    runs_base = strlen(path);

    if ((dp = opendir(path)) == NULL)
        return;

    while ((ep = readdir_visible(dp))) {
        strcat(path, ep->d_name);

        alloc_file(path, pf);

        if (f.content_size == 0)
            continue;

        log(path);
        puts(f.content);

        path[runs_base] = '\0';
    }

    free_file(pf);
    closedir(dp);
}

static void load_completions(char *path) {
    static volatile size_t comps_base;

    struct dirent *ep;
    DIR *dp;
    char *basename;

    char *line;

    log("loading completions");

    strcat(path, DP(COMPS_DIR));
    comps_base = strlen(path);

    if ((dp = opendir(path)) == NULL)
        return;

    while ((ep = readdir_visible(dp))) {
        strcat(path, ep->d_name);

        log(path);

        if ((line = read_line(path)) != NULL) {
            printf("complete -F %s bashdefault -o default %s\n", line,
                   (basename = get_base(path)));

            free(basename);
            free(line);
        }

        path[comps_base] = '\0';
    }

    closedir(dp);
}

static void load_keybinds(char *path) {
    static volatile size_t keys_base;

    struct dirent *ep;
    DIR *dp;
    char *basename;

    log("binding keybinds");

    strcat(path, DP(KEYS_DIR));

    keys_base = strlen(path);

    if (!path_exists(path))
        return;

    strcat(path, DIR_SEP KEYS_FULL_CTX_KEYF);

    if (path_exists(path)) {
        log("loading full-context keybinds");
        printf("bind -f %s\n", path);
    }

    path[keys_base] = '\0';

    strcat(path, DP(KEYS_CTX_DIR));

    if (!path_exists(path))
        return;

    if ((dp = opendir(path)) == NULL)
        return;

    log("loading contexed keybinds");

    while ((ep = readdir_visible(dp))) {
        strcat(path, ep->d_name);

        log(path);

        printf("bind -m %s -f %s\n", (basename = get_base(path)), path);
        free(basename);

        path[keys_base] = '\0';
    }

    closedir(dp);
}

static const stage_t stages[] = {
    load_envs,    load_cmds,        load_functions, load_aliases,
    load_runners, load_completions, load_keybinds,
};

static const size_t stages_sz = sizeof(stages) / sizeof(stages[0]);

int main(int argc, char **argv) {
    static volatile size_t path_base;
    static volatile unsigned char stage;
    char *path;

    if (argc < 2)
        return 1;

    path =
#ifdef ALLOW_ALLOCA
        alloca(PATH_MAX)
#else
        malloc(PATH_MAX)
#endif
        ;

#ifdef LOGGING
    if ((debug_load = getenv(DEBUG_LOAD) != NULL))
        puts(LOGGING_FN_DEF);

    log("baz version '$BAZ_LOADER_VERSION'");
#endif

    while (*++argv) {
        log("");

        path_base = strlen(*argv);
        strcpy(path, *argv);

        log(path);

        for (stage = 0; stage < stages_sz; ++stage) {
            stages[stage](path);
            path[path_base] = '\0';
        }
    }

#ifndef ALLOW_ALLOCA
    free(path);
#endif

    return 0;
}
