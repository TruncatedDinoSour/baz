#include "config.h"

#define FILE_IMPL
#define PATH_IMPL
#define SHELL_IMPL

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>

#include <sys/stat.h>

#include "macros.h"
#include "file.h"
#include "path.h"
#include "shell.h"

#ifdef LOGGING
static unsigned char debug_load;
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static File f = {0}, *pf = &f;

static char path[PATH_MAX + 1];
static size_t base;

static DIR *dp;
static struct dirent *ep;

typedef struct {
    void (*stage)(void);
    const char *path;
} stage_t;

LOAD_FN(envs) {
    log("loading environment variables");

    nputs("export");

    while ((ep = readdir_visible(dp))) {
        pathcat(ep->d_name);

        alloc_file(path, pf);
        escape_quotes(pf);

        printf(" %s=\"%s\"", ep->d_name, f.content);
        path[base] = '\0';
    }

    pnl();
}

LOAD_FN(cmds) {
    log("adding commands");

    printf("PATH=\"%s:$PATH\"\n", path);

    while ((ep = readdir_visible(dp))) {
        pathcat(ep->d_name);
        chmod(path, 0700);
        path[base] = '\0';
    }
}

LOAD_FN(functions) {
    log("loading functions");

    while ((ep = readdir_visible(dp))) {
        pathcat(ep->d_name);
        alloc_file(path, pf);

        printf("%s(){\n%s\n}\n", ep->d_name, f.content);

        path[base] = '\0';
    }
}

LOAD_FN(aliases) {
    log("loading aliases");

    nputs("alias");

    while ((ep = readdir_visible(dp))) {
        pathcat(ep->d_name);

        alloc_file(path, pf);
        escape_quotes(&f);

        printf(" %s=\"%s\"", ep->d_name, f.content);
        path[base] = '\0';
    }

    pnl();
}

LOAD_FN(runners) {
    log("running runners");

    while ((ep = readdir_visible(dp))) {
        pathcat(ep->d_name);
        alloc_file(path, pf);

        puts(f.content);
        path[base] = '\0';
    }
}

LOAD_FN(completions) {
    static char *line;

    log("loading completions");

    while ((ep = readdir_visible(dp))) {
        pathcat(ep->d_name);

        if ((line = read_line(path)) != NULL) {
            printf("complete -F %s bashdefault -o default %s\n", line,
                   ep->d_name);
            free(line);
        }

        path[base] = '\0';
    }
}

LOAD_FN(keybinds) {
    static struct dirent *epk;
    static DIR *dpk;

    log("binding keybinds");

    pathcat(DIR_SEP KEYS_FULL_CTX_KEYF);

    if (path_exists(path)) {
        log("loading full-context keybinds");
        printf("bind -f %s\n", path);
    }

    path[base] = '\0';
    pathcat(DP(KEYS_CTX_DIR));

    if ((!path_exists(path)) || ((dpk = opendir(path)) == NULL))
        return;

    log("loading contexed keybinds");

    while ((epk = readdir_visible(dpk))) {
        pathcat(epk->d_name);
        printf("bind -m %s -f %s\n", epk->d_name, path);
        path[base] = '\0';
    }

    closedir(dpk);
}

int main(int argc, const char *const *argv) {
    static size_t path_base;

    static unsigned char stage_num;
    static stage_t stage;

    static const stage_t stages[] = {
        {load_envs, DP(ENVS_DIR)},       {load_cmds, DP(CMDS_DIR)},
        {load_functions, DP(FUNCS_DIR)}, {load_aliases, DP(ALIASES_DIR)},
        {load_runners, DP(RUNS_DIR)},    {load_completions, DP(COMPS_DIR)},
    };
    static const size_t stages_sz = sizeof(stages) / sizeof(*stages);

    if (argc < 2)
        return 1;

#ifdef LOGGING
    if ((debug_load = getenv(DEBUG_LOAD) != NULL))
        puts(LOGGING_FN_DEF);

    log("baz version '$BAZ_LOADER_VERSION'");
#endif

    while (*++argv) {
        log("");
        memmove(path, *argv, (path_base = strlen(*argv)));
        log(path);

        for (stage_num = 0; stage_num < stages_sz; ++stage_num) {
            pathcat((stage = stages[stage_num]).path);

            if ((dp = opendir(path)) == NULL) {
                path[path_base] = '\0';
                continue;
            }

            base = strlen(path);
            stage.stage();

            path[path_base] = '\0';
            closedir(dp);
        }

        /* last stage -- keybinds
         * to be more efficient bc keybinds doesnt require
         * the keybinds dir to be open */

        pathcat(DP(KEYS_DIR));
        base = strlen(path);

        if (path_exists(path))
            load_keybinds();
    }

    puts("export PATH"); /* finish off commands, export path */

    free_file(pf);

    return 0;
}
