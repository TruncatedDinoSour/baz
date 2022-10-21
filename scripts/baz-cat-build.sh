#!/usr/bin/env sh

set -xe

main() {
    ${CXX:-clang++} $CXXFLAGS -flto -Ofast -ffunction-sections -fdata-sections -s -std=c++98 -Wall -Wextra -Wpedantic -Wshadow -Werror -pedantic -march=native -pipe -o baz-cat baz-cat.cc
    ${STRIP:-strip} --strip-all --remove-section=.note --remove-section=.gnu.version --remove-section=.comment --strip-debug --strip-unneeded baz-cat
}

main "$@"
