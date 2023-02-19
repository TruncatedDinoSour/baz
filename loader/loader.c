#include "config.h"

/* #define _POSIX_C_SOURCE 1 */

#define FILE_NO_STDDEF
#define STR_NO_STDDEF
#define PATH_NO_LIBS

#define FILE_IMPL
#define PATH_IMPL
#define STR_IMPL
#define SHELL_IMPL

#ifndef DELIM
#error define DELIM to the user delim
#define DELIM DEFAULT_DELIM
#endif

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>

#include "file.h"
#include "path.h"
#include "macros.h"
#include "str.h"
#include "shell.h"

#ifdef LOGGING
static unsigned char debug_load;
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

typedef void (*stage_t)(char *);

void load_envs(char *path) {
    size_t envs_base;

    struct dirent *ep;
    DIR *dp;
    File f;

    log("loading environment variables");

    strcat(path, DP(ENVS_DIR));
    envs_base = strlen(path);

    if ((dp = opendir(path)) == NULL)
        return;

    puts(".0(){\nlocal IFS=");

    while ((ep = readdir_visible(dp))) {
        strcat(path, ep->d_name);

        if ((f = read_file(path)).content == NULL)
            continue;

        if (f.content[f.content_size - 1] == '\n')
            f.content[f.content_size - 1] = '\0';

        log(path);

        printf("read -rd '' %s<<" DELIM "\n"
               "%s\n" DELIM "\nexport '%s'=\"${%s%%$'\\n'}\"\n",
               ep->d_name, f.content, ep->d_name, ep->d_name);

        close_file(&f);
        path[envs_base] = '\0';
    }
    puts("};.0");

    closedir(dp);
}

void load_cmds(char *path) {
    size_t cmds_base;

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

void load_functions(char *path) {
    size_t funs_base;

    struct dirent *ep;
    DIR *dp;
    File f;

    log("loading functions");

    strcat(path, DP(FUNCS_DIR));
    funs_base = strlen(path);

    if ((dp = opendir(path)) == NULL)
        return;

    while ((ep = readdir_visible(dp))) {
        strcat(path, ep->d_name);

        if ((f = read_file(path)).content == NULL)
            continue;

        log(path);
        printf("function %s(){\n%s\n}\n", ep->d_name, f.content);

        close_file(&f);
        path[funs_base] = '\0';
    }

    closedir(dp);
}

void load_aliases(char *path) {
    size_t alias_base;

    struct dirent *ep;
    DIR *dp;
    File f;
    Str alias_content;

    log("loading aliases");

    strcat(path, DP(ALIASES_DIR));

    alias_base = strlen(path);

    if ((dp = opendir(path)) == NULL)
        return;

    puts(".0(){");

    while ((ep = readdir_visible(dp))) {
        strcat(path, ep->d_name);

        log(path);

        f             = read_file(path);
        alias_content = escape_quotes(f.content, f.content_size);

        printf("alias %s=\"%s\"\n", ep->d_name, alias_content.string);
        path[alias_base] = '\0';

        close_file(&f);
        str_free(&alias_content);
    }

    puts("};.0");

    closedir(dp);
}

void load_runners(char *path) {
    size_t runs_base;

    struct dirent *ep;
    DIR *dp;
    File f;

    log("running runners");

    strcat(path, DP(RUNS_DIR));
    runs_base = strlen(path);

    if ((dp = opendir(path)) == NULL)
        return;

    while ((ep = readdir_visible(dp))) {
        strcat(path, ep->d_name);

        if ((f = read_file(path)).content == NULL)
            continue;

        log(path);
        puts(f.content);

        close_file(&f);
        path[runs_base] = '\0';
    }

    closedir(dp);
}

void load_completions(char *path) {
    size_t comps_base;

    struct dirent *ep;
    DIR *dp;
    char *basename;

    log("loading completions");

    strcat(path, DP(COMPS_DIR));
    comps_base = strlen(path);

    if ((dp = opendir(path)) == NULL)
        return;

    puts(".0(){\nlocal c");

    while ((ep = readdir_visible(dp))) {
        strcat(path, ep->d_name);

        log(path);
        printf(
            "read -r c<'%s'\ncomplete -F \"$c\" bashdefault -o default '%s'\n",
            path, (basename = get_base(path)));
        free(basename);

        path[comps_base] = '\0';
    }

    puts("};.0");

    closedir(dp);
}

void load_keybinds(char *path) {
    size_t keys_base;

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

        printf("bind -m '%s' -f '%s'\n", (basename = get_base(path)), path);
        free(basename);

        path[keys_base] = '\0';
    }

    closedir(dp);
}

int main(int argc, char **argv) {
    static const volatile stage_t stages[] = {
        load_envs,    load_cmds,        load_functions, load_aliases,
        load_runners, load_completions, load_keybinds,
    };

    static const volatile size_t stages_sz = sizeof(stages) / sizeof(stages[0]);

    size_t path_base;
    char *path;
    unsigned char stage;

    if (argc < 2) {
        fputs("no plugin dirs provided\n", stderr);
        return 1;
    }

    path = malloc(PATH_MAX);

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

    free(path);

    return 0;
}
