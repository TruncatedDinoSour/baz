<p align="center">
    <h1><img src="/logo.png" alt="baz" /></h1>
</p>

> a fast, easy, simple and lightweight plugin manager for GNU bash

# source code

-   [/baz](/baz) -- the plugin manager
-   [/loader/](/loader/)-- the baz plugin loader files
-   [/loader.sht](/loader.sht)-- the pure bash baz plugin loader

# credits

-   [@DannyBen](https://github.com/DannyBen) -- for creating an awesome tool for completion: https://github.com/DannyBen/completely

# dependencies

-   coreutils
-   BASH
-   git cli
-   optionally any c compiler ( to use the c loader insread of the bash one )
-   rlwrap ( optional, but its nice for special keys )
-   bash-completion ( optional, but its nice for well ... completion )

but you probably have all of them, maybe not `git`, but a lot of people do

# why `baz`

-   fast
    -   or at least faster than oh-my-bash and sheldon ( a plugin manager writen in rust ) : <https://blog.ari-web.xyz/b/comparison-between-the-oh-my-bash-and-baz-plugin-managers-for-gnu-bash/>
-   easy
-   small
-   simple
-   minimal
-   cross platform
    -   linux
        -   gentoo
        -   void
        -   debian
        -   ubuntu
        -   arch
        -   ...
    -   BSD
        -   FreeBSD
        -   OpenBSD
        -   ...
    -   windows
        -   git BASH
        -   windows subsystem for Linux ( WSL )
        -   wherever you can get BASH on
    -   android
    -   most likely ( needs testing, please contribute if you have the resources to do so :) )
        -   macOS
        -   iOS
-   GPLv3 licensing

# automatic setup

this script hosted on my files site will set up
baz automatically, but it might be unstable

```bash
$ cd /tmp

$ curl -fL 'https://ari-web.xyz/gh/baz/raw/main/scripts/baz_setup.sh' -o baz_setup.sh

$ bash baz_setup.sh

$ rm -f -- baz_setup.sh
```

or

```sh
cd /tmp && curl -fL 'https://ari-web.xyz/gh/baz/raw/main/scripts/baz_setup.sh' -o baz_setup.sh && bash baz_setup.sh && rm -f -- baz_setup.sh
```

script configuration :

-   `__BASH_RUNAS`: the `sudo` program ( `sudo` )
-   `PREFIX`: where to install `baz` to ( `/usr/bin` )
-   plus the [normal vars](#setup) like `BAZ_LOGGING_ENABLED`

# plugin indexing

you can leave a link to it in the [PLUGINS.md](/PLUGINS.md) file :)

# setup

-   clone the repo

```bash
$ git clone 'https://ari-web.xyz/gh/baz'
```

-   if you want logging enabled

```sh
export BAZ_LOGGING_ENABLED=1
```

-   if non-zero ( _its normal dw_ ) exit codes bother you on the loader run this :

```sh
export BAZ_ENSURE_OK=1
```

-   if you never want to use the C loader

```sh
export BAZ_NO_CC=1
```

-   set the c compiler ( has to at least support `-o <outfile>` and `-D MACRO="value"` flags )

```sh
export CC='clang'
```

-   set the c flags

```sh
export CFLAGS='-O3 -s'
```

-   set the stripper ( if none is set the binary wont be stripped )

```sh
export STRIP='llvm-strip'
```

-   set the stripper flags

```sh
export STRIPFLAGS='--remove-section=.note --strip-all'
```

-   install the script anywhere, or even run it standalone
    -   running standalone means just running it
    -   installing could be done in for example `/usr/local/bin`:

```bash
$ su -c 'install -Dm755 baz /usr/local/bin'
```

-   setup

```bash
$ baz setup
```

Or

```bash
$ ./baz setup
```

-   add this to your `~/.bashrc` :

```bash
export BAZ_LOADER_ENABLED=true
_baz_loader="$HOME/.local/share/baz/loader.sh"

[ ! -f "$_baz_loader" ] || source "$_baz_loader"
```

this is the main loader for plugins

# installing completion

```bash
$ ./scripts/comp.sh
```

# uninstallation

Uninstall the `baz` binaries and then just run:

```sh
$ sudo rm -rf ~/.local/share/baz*
```

# help page

```bash
$ baz help
```

# upgrading

Just upgrade your package or pull the repo
and run :

```bash
$ ./baz upgrade
```

and if you also have completion, rerun the completion
script :

```bash
$ ./scripts/comp.sh
```

# other docs

-   [doc folder](/doc)

# examples

-   [examples folder](/examples)
-   [my example plugin repo](https://github.com/TruncatedDinosour/baz-example-plugin)

# debugging

-   set `BAZ_DEBUG` to any value to any value to debug the `baz` cli with tracing
-   set `BAZ_DEBUG_LOAD` to any value to debug the loader
    -   this only works if `BAZ_LOGGING_ENABLED` was set during setup time
-   add `set -x` before and `set +x` after the loader to enable tracing
