# baz loader specification

> this specification goes for baz `v5.4.0` and up

## environment variables

there should be at least these environment variables :

-   `BAZ_LOADER_VERSION` -- your `baz` plugin loader version, should be the same as `baz` version

## functions

most function names can be mangled, only these cannot :

-   `baz_load_plugin_low <plugin-directory>` -- this function takes `plugin-directory` as its argument and loads that plugin ( keep in mind **_THIS IS A VERY INPUT-SENSITIVE_** function, read more in `baz help` )
-   `baz_loader` -- the loader "main" function

## conditions

your plugin loader should take these environment variables into consideration at runtime :

-   `BAZ_LOADER_ENABLED` -- if this environment variable is not set, you should not start loading your plugins
-   `BAZ_DEBUG_LOAD` -- this should log what its loading if logging is enabled

## pre-processor

-   all expansions should be `#{...}` ( e.g. `#{BAZ_VER}` )

your pre-processor variables are only going to be used
for your loader, if you want, here are some nice variables to have:

-   `BAZ_DATA_DIR` -- the baz data directory ( `~/.local/share/baz/` )
-   `BAZ_PLUG_DIR` -- the baz plugin directory ( `$BAZ_PLUG_DIR/plugins` )
-   `BAZ_RDELIM` -- random delimiter to be used in environment variable expansion
-   `BAZ_VER` -- baz version
-   `BAZ_LOGGING_FN` -- if logging is enabled -- the logging function for the loader, else pass
    -   logging is enabled if `BAZ_LOGGING_ENABLED` is set
-   `BAZ_LOG` -- if logging is enabled -- call the logging function, else pass
-   `BAZ_NOP` -- expands to `:` if `BAZ_ENSURE_OK` is set at pre-processing step
-   `BAZ_RD` -- expands to `read -rd ''`

the pre-processor should also handle optimisation of size and mangling of function names,
it pre-processes `*.sht` (**Sh**ell **T**emplate) files

## function name mangling

the pre-processor should mangle all function names starting with `_`,
the mangle can be anything, but preferred is `.<incremental number starting from 0>`,
for example: `.0`, `.1` and so on

yes, BASH allows `.` in function names

## directories and files

your loader should implement loading of all files and directories
specified in the [plugin structure documentation](/doc/PLUGIN_FOLDER_STRUCTURE.md)

## licensing

preferred licenses :

-   WTFPL ( `Do what the fuck you want to Public license` -- <http://www.wtfpl.net/> )
-   GPLv3 ( `GNU general public license version 3` -- <https://www.gnu.org/licenses/gpl-3.0.en.html> )

## load order

-   environment variables
-   commands
-   functions
-   aliases
-   runners
-   BASH completions
-   key bindings

## tips

-   optimise for speed no matter what it takes
    -   <https://blog.ari-web.xyz/b/gnu-bash-script-and-general-code-optimisation-tips/>
-   code size matters, but if its at the cost of speed, prioritize speed
-   code style matters even if its going to be mangled
-   contribute to open source, baz is an open source project afterall
-   use tags : `baz` ,`baz-plugin` and / or `baz-loader`
