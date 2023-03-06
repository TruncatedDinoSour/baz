#ifndef _SHELL_H
#define _SHELL_H
#ifndef SHELL_NO_STR
#include "str.h"
#endif

static void escape_quotes(Str *, const char *, size_t);

#ifdef SHELL_IMPL
static void escape_quotes(Str *str, const char *shell, size_t init_size) {
    if (init_size > str->len) {
        str->string = realloc(str->string, (init_size += STR_GROWTH));
        str->len    = init_size;
    }

    str->idx = 0;

    if (!init_size) {
        *str->string = '\0';
        return;
    }

    while (*shell != '\0') {
        if (*shell == '"')
            str_append(str, '\\');

        str_append(str, *shell++);
    }

    str->string[str->idx - (str->string[str->idx - 1] == '\n')] = '\0';
}
#endif /* SHELL_IMPL */
#endif /* _SHELL_H */
