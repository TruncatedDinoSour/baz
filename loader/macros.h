#ifndef _MACROS_H
#define _MACROS_H
#include "config.h"
#define DP(name) DIR_SEP name DIR_SEP
#ifdef LOGGING
#define log(msg)    \
    if (debug_load) \
    printf("%s \"%s\"\n", LOGGING_FN, msg)
#else
#define log(msg)
#endif /* LOGGING */
#endif /* _MACROS_H */
