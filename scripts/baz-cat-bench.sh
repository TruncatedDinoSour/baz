#!/usr/bin/env bash

set -e

log() { echo "$1" >&2; }

log 'Generating ranges and inputs'

INPUT_C=1000000
RANGE_C=100

INPUT="$(seq -- "$INPUT_C")"
RANGE="$(seq -- "$RANGE_C")"

bench() {
    log "Benchmarking '$1' ($2)"

    printf '%s' "STATS[\"$2\"] = ("
    for _ in $RANGE; do
        printf '%G, ' "$( (echo "$INPUT" | command time -f '%e' "$1" >/dev/shm/baz-cat-bench) 2>&1)"
    done
    echo ")"
}

main() {
    {
        cat <<EOF
from statistics import median
from typing import Dict, Tuple

STATS: Dict[str, Tuple[int, ...]] = {}


def print_stats() -> None:
    best_median: float = float("inf")
    best_average: float = float("inf")

    worst_median: float = 0
    worst_average: float = 0

    best_lang: str = "unknown"
    worst_lang: str = "unknown"

    for lang, data in STATS.items():
        data_median: float = median(data)
        data_average: float = sum(data) / $INPUT_C

        if data_median > worst_median and data_average > worst_average:
            worst_lang = lang

        if data_median < best_median and data_average < best_average:
            best_lang = lang

        if data_median < best_median:
            best_median = data_median

        if data_median > worst_median:
            worst_median = data_median

        if data_average < best_average:
            best_average = data_average

        if data_average > worst_average:
            worst_average = data_average

        print(
            f"""
-----------------------------------------------------
Statistics for {lang!r} -- $RANGE_C/$INPUT_C

Median: {data_median}
    Median per unit: {data_median / $INPUT_C}
    Median per run:  {data_median / $RANGE_C}

Average: {data_average}
    Average per unit: {data_average / $INPUT_C}
    Average per run:  {data_average / $RANGE_C}
-----------------------------------------------------
"""
        )

    print(f"\nWinner: {best_lang} with med({best_median}), avg({best_average})")
    print(f"\nLoser:  {worst_lang} with med({worst_median}), avg({worst_average})")

EOF

        for file in baz-cat-impls/*; do
            if [ ! -f "$file" ] || ! echo "$file" | grep 'baz-cat\.' >/dev/null; then
                log "Skipping '$file'"
                continue
            fi

            local lang="${file##*.}"

            log "Building '$file' ($lang)"
            ./scripts/baz-cat-build.sh "$lang" &>/dev/null

            bench './baz-cat' "$lang"
        done

        log "Running coreutils 'cat'"
        bench "$(which cat)" 'coreutils'

        echo 'print_stats()'
    } | python3
}

main "$@"
