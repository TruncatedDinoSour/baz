#ifndef _SHELL_H
#define _SHELL_H
#include "str.h"

Str escape_quotes(char *, size_t);

#ifdef SHELL_IMPL
Str escape_quotes(char *shell, size_t size) {
    Str str = {STR_GROWTH, 0, NULL};
    str.len += size;
    str.string = malloc(str.len);

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
