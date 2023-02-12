#!/usr/bin/env sh

set -e

main() {
    echo 'did you change the version ??'
    printf 'press enter to mean yes'
    read -r _

    echo 'okay ... i will sleep for couple of seconds just in case'
    sleep 4s

    git add -A
    git commit -sa
    git push -u origin "$(git rev-parse --abbrev-ref HEAD)"
}

main "$@"
