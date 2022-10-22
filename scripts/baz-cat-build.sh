#!/usr/bin/env sh

set -e

export AC="${AC:-fasm}" \
    CC="${CC:-clang}" \
    CXX="${CXX:-clang++}" \
    STRIP="${STRIP:-strip}" \
    O="${O:-baz-cat}"

# Helpers

err() {
    echo "error: $1" >&2
    exit 1
}

use() {
    set +x

    for dep in "$@"; do
        command -v -- "$dep" >/dev/null ||
            err "Required dependency not satisfied: $dep"
    done

    set -x
}

build_c_like() {
    use "$1" "$STRIP"

    # shellcheck disable=SC2086
    ${1} ${2} -flto -Ofast -ffunction-sections -fdata-sections -s -std="$3" -Wall -Wextra -Wpedantic -Wshadow -Werror -pedantic -march=native -pipe -o "$O" "$4"
    ${STRIP} --strip-all --remove-section=.note --remove-section=.gnu.version --remove-section=.comment --strip-debug --strip-unneeded "$O"
}

# Builders

build_asm() {
    use "$AC"
    ${AC} "$1" "$O"
}

build_c() { build_c_like "$CC" "$CFLAGS" 'c89' "$1"; }
build_cpp() { build_c_like "$CXX" "$CXXFLAGS" 'c++98' "$1"; }

# Main

main() {
    [ "$#" -lt 1 ] && err 'Supply the type in the 1st arg, this will be used as the file extension' >&2

    file="baz-cat-impls/baz-cat.$1"
    builder="build_$1"

    [ -f "$file" ] || err 'No valid file extension supplied, no implementation for it'

    command -v -- "$builder" >/dev/null || err 'No builder implemented for this file type'

    set -x
    "$builder" "$file"
}

main "$@"
