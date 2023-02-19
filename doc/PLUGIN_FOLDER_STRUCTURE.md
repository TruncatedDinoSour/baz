# baz plugin folder structure

as you can in in [the example plugin](/examples/example_plugin) folder
there is a specific structure for files and folders,
what do they all mean ?

## baz.env

baz.env, also known as index file is the
main plugin file, it contains all metadata
about the plugin, more on that in [the index file doc](/doc/BAZ_ENV.md)

## plugin

`plugin` is not a standard folder, it's
defined by the index file which contains
`BAZP_SRC` which is the plugin itself,
where it all goes

### plugin/aliases

this folder has all aliases defined
by baz, by ... you, the folder structure
goes like this :

```
aliases/
    | <alias name>
    | <alias name>
    | <alias name>
    | <alias name>
```

alias name can be anything with uppercase and lowercase
letters and \_, the files should include
the alias content, for example :

```
aliases/
    | hello
```

-   aliases/hello

```bash
echo 'hi'
```

is the same as

```bash
alias hello="echo 'hi'"
```

### plugin/commands

this directory should include executable
files / scripts that will get added to `$PATH`,
its naming is the same as the aliases, by which
i mean :

```
commands/
    | <command name>
    | <command name>
    | <command name>
    | <command name>
```

and it has the same naming rules as aliases

expands to

```sh
export PATH="/path/to/commands:$PATH"
```

### plugin/completions

this directory should include all completions
and what functions to use for them as the files
content, the folder structure should look like
this :

```
completions/
    | <command name>
    | <command name>
    | <command name>
    | <command name>
```

in files `<command name>` should be a function name
( should be defined in `functions/` directory ) and nothing
else, it will be called as the completions function

for example :

```
completions/
    | arch
```

##### completions/arch

```bash
__arch_comp
```

##### functions/\_\_arch_comp

```bash
# mailman arch completion                                  -*- shell-script -*-

# Try to detect whether this is the mailman "arch" to avoid installing
# it for the coreutils/util-linux-ng one.
_have mailmanctl &&
    _arch() {
        local cur prev words cword split
        _init_completion -s || return

        case $prev in
        -w | -g | -d | --welcome-msg | --goodbye-msg | --digest)
            COMPREPLY=($(compgen -W 'y n' -- "$cur"))
            return
            ;;
        --file)
            _filedir
            return
            ;;
        esac

        $split && return

        if [[ $cur == -* ]]; then
            COMPREPLY=($(compgen -W '$(_parse_help "$1")' -- "$cur"))
        else
            local args=$cword
            for ((i = 1; i < cword; i++)); do
                if [[ ${words[i]} == -* ]]; then
                    ((args--))
                fi
            done
            case $args in
            1)
                _xfunc list_lists _mailman_lists
                ;;
            2)
                _filedir
                ;;
            esac
        fi

    } && complete -F _arch arch

# ex: filetype=sh
```

this will generate completions for `arch` command

### plugin/environments

this directory includes environment variable
definitions, it also has the same naming and
structure as aliases :

```
environments/
    | <environment variable name>
    | <environment variable name>
    | <environment variable name>
    | <environment variable name>
```

same rules for naming as aliases and commands

it gets expanded like this :

```bash
export '<environment variable name>'="<content of file>"
```

### plugin/functions

this directory includes function definitions,
it also has the same naming and structure as aliases :

```
functions/
    | <funnction name>
    | <funnction name>
    | <funnction name>
    | <funnction name>
```

same rules for naming as aliases, commands and environments

it gets expanded like this :

```bash
function <function_name>() {
    <your code>
}
```

### plugin/keybinds

this directory includes [GNU readline syntax](https://www.gnu.org/software/bash/manual/html_node/Readline-Init-File-Syntax.html)
files, they will get `bind`ed

folder structure should look like this :

```
keybinds/
    | all.rl
    | bindings/
        | <context name>
        | <context name>
        | <context name>
        | <context name>
```

-   `all.rl`

this includes full-context bindings, it will
work in any context ( keymap )

-   `bindings`

this directory includes files which again files
with readline syntax but they get binded to a specific
context, for example `vi-command`, `emacs-meta`, etc.
the context should be the name of the file

`all.rl` basically expands to :

```bash
bind -f 'all.rl"
```

`bindings/...` expands to :

```bash
bind -m '<context name>' -f '<context name ( as path )>'
```

### plugin/runners

this directory includes files that will be sourced / ran
on load :

```
runners/
    | <runner name>
    | <runner name>
    | <runner name>
    | <runner name>
```

it gets expanded like this :

```bash
source <runner name>
```

### plugin/hooks

you can store hooks here,
they are either ran or sourced after or before a
certain action, as of now I have restricted it to these hooks :

-   pre-install
    -   post-install
-   pre-update
    -   post-update
-   pre-uninstall

these hooks will be ran with bash ( `bash <hook-name>` ),
if you want more low level control and want to affect
baz from the core just append `-source` to your hook name :

-   pre-install-source
    -   post-install-source
-   pre-update-source
    -   post-update-source
-   pre-uninstall-source

these will be sourced ( `source <hook-name>` )

also you an append `-pre` to run before the hook and
`-post` after the hook, they will be sourced :

-   pre-install-pre
-   pre-install-post
-   pre-update-pre
-   pre-update-post
-   pre-uninstall-pre
-   pre-uninstall-post

#### order

-   Pre (source)
-   Hook (run)
-   Source (source)
-   Post (source)
