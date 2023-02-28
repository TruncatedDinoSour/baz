#ifndef _SHELL_H
#define _SHELL_H
#ifndef SHELL_NO_STR
#include "str.h"
#endif

static Str escape_quotes(char *, const size_t);

#ifdef SHELL_IMPL
static Str escape_quotes(char *shell, const size_t size) {
    Str str = {STR_GROWTH, 0, NULL};
    str.len += size;
    str.string = mem_alloc(str.len);

    if (!size) {
        str.string[0] = '\0';
        return str;
    }

    while (*shell != '\0') {
        if (*shell == '"')
            str_append(&str, '\\');

        str_append(&str, *shell++);
    }

    str.string[str.idx - (str.string[str.idx - 1] == '\n')] = '\0';

    return str;
}
#endif /* SHELL_IMPL */
#endif /* _SHELL_H */
