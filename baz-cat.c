#include <stdio.h>

/*
 * Improved performance, C89 compatible cat(1) for `baz` usage,
 * this will read input and then print it
 */

int main(void) {
    static char c;

    /*
     * This is kind of an implementation of puts lol,
     * it doesn't really slow down the program, but
     * I have to read it char-by-char to support
     * unicode
     */
    while ((c = getchar()) != EOF)
        putchar(c);

#ifdef MANUAL_FLUSH
    /*
     * Enable this with -DMANUAL_FLUSH if you want
     * to manually flush stdout on call, although
     * C already has auto flushing to cover most
     * cases and this just slows the program down
     */

    fflush(stdout);
#endif

    return 0;
}
