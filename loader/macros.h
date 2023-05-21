#ifndef _MACROS_H
#define _MACROS_H
#define DP(name) DIR_SEP name DIR_SEP
#ifdef LOGGING
#define log(msg)    \
    if (debug_load) \
    printf("%s \"%s\"\n", LOGGING_FN, msg)
#else
#define log(msg)
#endif /* LOGGING */
#define nputs(s) fputs(s, stdout)
#define pnl()    putchar(newline)
#if defined(BSD) || defined(__bsd__) || defined(__BSD__) ||             \
    defined(__FreeBSD__) || defined(__OpenBSD__) || defined(FreeBSD) || \
    defined(OpenBSD)
#define pathcat(what) strlcat(path, what, PATH_MAX)
#else
#define pathcat(what) strncat(path, what, PATH_MAX)
#endif /* BSD */
#define LOAD_FN(what) static void load_##what(void)
#endif /* _MACROS_H */
