# baz loader

this loader has 2 parts

-   loader template ( wrapper ) at [loader.sht](loader.sht)
-   loader itself ( c89 code ) at [loader.c](loader.c)

`loader.sht` is needed to interract with the loader itself
and implement the baz loader standard

use `config.h` to configure it

## example compilation

```sh
$ clang loader/loader.c -std=c89 -s -Wall -Wextra -Wpedantic -pedantic -Werror -Wshadow -Ofast -flto -o load
```

want very optimised flags ? see <https://raw.githubusercontent.com/TruncatedDinosour/dotfiles-cleaned/gentoo/dotfiles/config/baz/genoo-cflags.env>
