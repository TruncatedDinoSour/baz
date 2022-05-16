#!/usr/bin/env sh

set -xe

main() {
    compdir="$HOME/.local/share/bash-completion/completions"

    mkdir -p "$compdir"
    cp completions/baz.bash "$compdir/baz"
}

main "$@"
