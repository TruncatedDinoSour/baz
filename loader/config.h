#ifndef _CONFIG_H
#define _CONFIG_H
static const volatile char newline = '\n';
#define DIR_SEP            "/"
#define ENVS_DIR           "environments"
#define CMDS_DIR           "commands"
#define FUNCS_DIR          "functions"
#define ALIASES_DIR        "aliases"
#define RUNS_DIR           "runners"
#define COMPS_DIR          "completions"
#define KEYS_DIR           "keybinds"
#define KEYS_FULL_CTX_KEYF "all.rl"
#define KEYS_CTX_DIR       "bindings"
#define DEBUG_LOAD         "BAZ_DEBUG_LOAD"
#define LOGGING_FN         ".1"
#define LOGGING_FN_DEF     LOGGING_FN "(){ echo \"" LOGGING " $1\">&2;}"
#define STR_GROWTH         64
#define LINE_GROWTH        64
/* #define ALLOW_ALLOCA allow usage of alloca() if appropriate */
#endif /* _CONFIG_H */
