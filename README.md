# Baz

> A lightweight plugin manager for GNU bash

# Source code

-   [/baz](/baz) -- The plugin manager
-   [/loader.sht](/loader.sht) -- The baz plugin loader

# Credits

-   [@DannyBen](https://github.com/DannyBen) -- For creating an awesome tool for completion: https://github.com/DannyBen/completely

# Dependencies

-   GNU coreutils
-   GNU bash
-   Git cli
-   Linux
-   Rlwrap (optional, but it's nice for special keys)
-   Bash-completion (optional, but it's nice for well.. Completion)

# Why `baz`

-   Fast
    -   <https://blog.ari-web.xyz/b/comparison-between-the-oh-my-bash-and-baz-plugin-managers-for-gnu-bash/>
-   Easy
-   Small
-   Minimalistic
-   GPL3 licensing

# Automatic setup

This script hosted on my files site will set up
baz automatically, but it might be unstable

```bash
$ curl -o baz_setup.sh 'https://files.ari-web.xyz/files/baz_setup.sh'

$ bash baz_setup.sh
```

Configuration:

-   `__BASH_RUNAS`: The `sudo` program (sudo)
-   `PREFIX`: Where to install baz (/usr/bin)

# Plugin indexing

You can leave a link to it in the [PLUGINS.md](/PLUGINS.md) file :)

# Setup

-   Clone the repo

```bash
$ git clone 'https://ari-web.xyz/gh/baz'
```

-   If you want debug logging support in the loader run this:

```sh
export BAZ_LOGGING_ENABLED=1
```

-   If non-zero (it's normal dw) exit codes bother you on the loader run this:

```sh
export BAZ_ENSURE_OK=1
```

-   Install the script anywhere, or even run it standalone
    -   Running standalone means just running it
    -   Installing could be done in for example `/usr/local/bin`:

```bash
$ su -c 'install -Dm755 baz /usr/local/bin'
```

-   Setup

```bash
$ baz setup
```

Or

```bash
$ ./baz setup
```

-   Add this to your `~/.bashrc`:

```bash
export BAZ_LOADER_ENABLED=true
_baz_loader="$HOME/.local/share/baz/loader.sh"

[ ! -f "$_baz_loader" ] || source "$_baz_loader"
```

This is the main loader for plugins

# Installing completion

```bash
$ ./scripts/comp.sh
```

# Uninstallation

Uninstall the `baz` binaries and then just run:

```
$ sudo rm -rf ~/.local/share/baz*
```

# Help page

```bash
$ baz help
```

# Upgrading

Just upgrade your package or pull the repo
and run:

```bash
$ ./baz upgrade
```

And if you also have completion, rerun the completion
script:

```bash
$ ./scripts/comp.sh
```

# Other docs

-   [Doc folder](/doc)

# Examples

-   [Examples folder](/examples)
-   [My example plugin repo](https://github.com/TruncatedDinosour/baz-example-plugin)

# Debugging

-   Set `BAZ_DEBUG` to any value to any value to debug the `baz` cli with tracing
-   Set `BAZ_DEBUG_LOAD` to any value to debug the loader
    -   This only works if `BAZ_LOGGING_ENABLED` was set during setup time
-   Add `set -x` before and `set +x` after the loader to enable tracing
