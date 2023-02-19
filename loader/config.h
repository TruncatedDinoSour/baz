#ifndef _CONFIG_H
#define _CONFIG_H
#define DEFAULT_DELIM \
    "_____DEFAULT.UNSAFE_239983baecedfa142b90e35024048_DONT.USE.ME_____"
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
#define NUM_MAX_LEN        12
#define STR_GROWTH         32
#endif /* _CONFIG_H */
