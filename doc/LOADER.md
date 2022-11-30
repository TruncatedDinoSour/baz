# Baz loader specification

> This specification goes for baz `v5.4.0` and up

## Environment variables

There should be at least these two environment variables:

-   `BAZ_LOADER_VERSION` -- Your `baz` plugin loader version, should be the same as `baz` version
-   `BAZ_NL` -- The newline used in environment variables

## Functions

Most function names can be mangled, only these cannot:

-   `baz_load_plugin_low <plugin-directory>` -- This function takes `plugin-directory` as its argument and loads that plugin
-   `baz_loader` -- The loader "main" function

## Conditions

Your plugin loader should take these environment variables into consideration at runtime:

-   `BAZ_LOADER_ENABLED` -- If this environment variable is not set, you should not start loading your plugins
-   `BAZ_DEBUG_LOAD` -- This should log what it's loading if logging is enabled

## Pre-processor

-   All expansions should be `#{...}` (e.g. `#{BAZ_VER}`)

Your pre-processor variables are only going to be used
for your loader, if you want, here are some nice variables to have:

-   `BAZ_DATA_DIR` -- The baz data directory (`~/.local/share/baz/`)
-   `BAZ_PLUG_DIR` -- The baz plugin directory (`$BAZ_PLUG_DIR/plugins`)
-   `BAZ_RDELIM` -- Random delimiter to be used in environment variable expansion
-   `BAZ_VER` -- Baz version
-   `BAZ_LOGGING_FN` -- If logging is enabled -- the logging function for the loader, else pass
    -   Logging is enabled if `BAZ_LOGGING_ENABLED` is set
-   `BAZ_LOG` -- If logging is enabled -- call the logging function, else pass
-   `BAZ_NOP` -- Expands to `:` if `BAZ_ENSURE_OK` is set at pre-processing step
-   `BAZ_RD` -- Expands to `read -rd ''`

The pre-processor should also handle optimisation of size and mangling of function names,
it pre-processes `*.sht` (**Sh**ell **T**emplate) files

## Function name mangling

The pre-processor should mangle all function names starting with `_`,
the mangle can be anything, but preferred is `.<incremental number starting from 0>`,
for example: `.0`, `.1` and so on

Yes, BASH allows `.` in function names

## Directories and files

Your loader should implement loading of all files and directories
specified in the [plugin structure documentation](/doc/PLUGIN_FOLDER_STRUCTURE.md)

## Licensing

Preferred licenses:

-   WTFPL (`Do what the fuck you want to Public license` -- <http://www.wtfpl.net/>)
-   GPLv3 (`GNU general public license version 3` -- <https://www.gnu.org/licenses/gpl-3.0.en.html>)

## Load order

-   Environment variables
-   Commands
-   Functions
-   Aliases
-   Runners
-   BASH completions
-   Key bindings

## Tips

-   Optimise for speed no matter what it takes
    -   <https://blog.ari-web.xyz/b/gnu-bash-script-and-general-code-optimisation-tips/>
-   Code size matters, but if it's at the cost of speed, prioritize speed
-   Code style matters even if it's going to be mangled
-   Contribute to open source, baz is an open source project afterall
-   Use tags: `baz` ,`baz-plugin` and/or `baz-loader`
