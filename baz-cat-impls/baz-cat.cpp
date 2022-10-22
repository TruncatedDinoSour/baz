#include <string>
#include <iostream>

/*
 * Improved performance, C++98 compatible cat(1) for `baz` usage,
 * this will read input and then print it
 */

int main(void) {
    static std::string s, l;

    while (true) {
        if (!std::getline(std::cin, l))
            break;

        s += l + '\n';
    }

    std::cout << s;

#ifdef MANUAL_FLUSH
    /*
     * Enable this with -DMANUAL_FLUSH if you want
     * to manually flush stdout on call, although
     * C++ already has auto flushing to cover most
     * cases and this just slows the program down
     */

    std::cout.flush();
#endif

    return 0;
}
