#ifndef _STR_H
#define _STR_H
#ifndef STR_NO_STDDEF
#include <stddef.h>
#endif
#include "config.h"

typedef struct {
    size_t len, idx;
    char *string;
} Str;

void str_append(Str *, char);
void str_free(Str *);

#ifdef STR_IMPL
void str_append(Str *str, char chr) {
    if (str->idx >= str->len) {
        str->len += STR_GROWTH;
        str->string = realloc(str->string, str->len);
    }

    str->string[str->idx++] = chr;
}

void str_free(Str *str) { free(str->string); }
#endif /* STR_IMPL */
#endif /* _STR_H */
