#ifndef _SHELL_H
#define _SHELL_H
#ifndef SHELL_NO_FILE
#include "file.h"
#endif

static void escape_quotes(File *);

#ifdef SHELL_IMPL
static void escape_quotes(File *file) {
    char *new_content, *old_ptr, *new_ptr;

    new_content = malloc((file->content_size *= 2));
    old_ptr = file->content, new_ptr = new_content;

    while (*old_ptr) {
        if (*old_ptr == '"')
            *new_ptr++ = '\\';

        *new_ptr++ = *old_ptr++;
    }

    *(new_ptr - (*(new_ptr - 1) == '\n')) = '\0';

    free(file->content);
    file->content = new_content;
}
#endif /* SHELL_IMPL */
#endif /* _SHELL_H */
