#ifndef _STR_H
#define _STR_H
#ifndef STR_NO_STDDEF
#include <stddef.h>
#endif

typedef struct {
    size_t len, idx;
    char *string;
} Str;

static void str_append(Str *, const char);
static void str_free(Str *);

#ifdef STR_IMPL
static void str_append(Str *str, const char chr) {
    if (str->idx == str->len)
        str->string = mem_realloc(str->string, (str->len += STR_GROWTH));

    str->string[str->idx++] = chr;
}

static void str_free(Str *str) { mem_free(str->string); }
#endif /* STR_IMPL */
#endif /* _STR_H */
