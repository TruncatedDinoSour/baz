#ifndef _SHELL_H
#define _SHELL_H
#include "file.h"

static void escape_quotes(File *const);

#ifdef SHELL_IMPL
static void escape_quotes(File *const file) {
    static size_t quotes;
    static char *new_content, *old_ptr, *new_ptr;

    quotes = 0;

    old_ptr     = file->content;
    new_content = file->content;

    while (*new_content)
        quotes += (*new_content++ == '"');

    if (!quotes) {
        file->content[file->content_size -
                      (file->content[file->content_size - 1] == '\n')] = '\0';
        return;
    }

    new_content = malloc((file->content_size += quotes));
    new_ptr     = new_content;

    while (*old_ptr) {
        if (*old_ptr == '"')
            *new_ptr++ = '\\';

        *new_ptr++ = *old_ptr++;
    }

    *(new_ptr - (*(new_ptr - 1) == '\n')) = '\0';

    free_file(file);
    file->content = new_content;
}
#endif /* SHELL_IMPL */
#endif /* _SHELL_H */
