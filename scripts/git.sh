#!/usr/bin/env sh

set -e

main() {
    echo 'Did you change the version??'
    printf 'Press enter to mean yes'
    read -r

    echo 'Okay.. I will sleep for couple of seconds just in case'
    sleep 4s

    git add -A
    git commit -sa
    git push -u origin "$(git rev-parse --abbrev-ref HEAD)"
}

main "$@"
