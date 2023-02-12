#!/usr/bin/env bash

set -e

export __BASH_RUNAS="${__BASH_RUNAS:-sudo}"
export PREFIX="${PREFIX:-/usr/bin}"

err() {
    echo "$1" >&2
    exit 1
}

use() {
    for dep in "$@"; do
        command -v -- "$dep" >/dev/null ||
            err "required dependency not satisfied : $dep"
    done
}

main() {
    echo 'beginning setting up baz'

    # shellcheck disable=SC2015
    command -v baz && err 'you already have baz installed' || true
    [ -d '.git' ] && err 'you must not be in a git repository to set baz up'

    use git "${__BASH_RUNAS}" install sh bash

    echo 'pre-cleanup'
    rm -rf -- baz

    echo 'cloning git repository'
    git clone 'https://ari-web.xyz/gh/baz'
    cd baz

    if [ ! -x 'baz' ]; then
        echo 'making baz executable'
        chmod 755 baz
    fi

    echo 'installing baz'
    ${__BASH_RUNAS} install -Dm755 baz "$PREFIX"

    echo 'setting baz up'
    ./baz setup

    [ "$yn" ] || read -rp 'do you want to add loader lines to bashrc ? [y/n] >>> ' yn

    if [ "$yn" = 'y' ]; then
        use tee

        echo 'adding lines to bashrc'
        tee -a ~/.bashrc <<EOF

# load baz
export BAZ_LOADER_ENABLED=true
_baz_loader="\$HOME/.local/share/baz/loader.sh"

# shellcheck disable=SC1090
[ ! -f "\$_baz_loader" ] || source "\$_baz_loader"

EOF
    fi

    echo 'adding completions'
    bash ./scripts/comp.sh

    echo 'cleaning up'
    cd ..
    rm -rf -- baz

    echo "done setting up baz, make sure '$PREFIX' is in \$PATH"
}

main "$@"
