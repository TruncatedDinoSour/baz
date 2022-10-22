#!/usr/bin/env sh

set -xe

main() {
    ${CC:-clang} $CFLAGS -flto -Ofast -ffunction-sections -fdata-sections -s -std=c89 -Wall -Wextra -Wpedantic -Wshadow -Werror -pedantic -march=native -pipe -o baz-cat baz-cat.c
    ${STRIP:-strip} --strip-all --remove-section=.note --remove-section=.gnu.version --remove-section=.comment --strip-debug --strip-unneeded baz-cat
}

main "$@"
