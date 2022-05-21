# Baz plugin folder structure

As you can in in [the example plugin](/examples/example_plugin) folder
there is a specific structure for files and folders,
what do they all mean?

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

This folder has all aliases defined
by baz, by... You, the folder structure
goes like this:

```
aliases/
    | <alias name>
    | <alias name>
    | <alias name>
    | <alias name>
```

Alias name can be anything with uppercase and lowercase
letters and \_, the files should include
the alias content, for example:

```
aliases/
    | hello
```

- aliases/hello

```bash
echo 'hi'
```

Is the same as

```bash
alias hello="echo 'hi'"
```

### plugin/commands

This directory should include executable
files/scripts that will get added to `$PATH`,
its naming is the same as the aliases, by which
I mean:

```
commands/
    | <command name>
    | <command name>
    | <command name>
    | <command name>
```

And it has the same naming rules as aliases

### plugin/completions

This directory should include all completions
and what functions to use for them as the file's
content, the folder structure should look like
this:

```
completions/
    | <command name>
    | <command name>
    | <command name>
    | <command name>
```

In files `<command name>` should be a function name
(should be defined in `functions/` directory) and nothing
else, it will be called as the completions function.

For example:

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

This will generate completions for `arch` command

### plugin/environments

This directory includes environment variable
definitions, it also has the same naming and
structure as aliases:

```
environments/
    | <environment variable name>
    | <environment variable name>
    | <environment variable name>
    | <environment variable name>
```

Same rules for naming as aliases and commands

It gets expanded like this:

```bash
export <environment variable name>=<content of file>
```

### plugin/functions

This directory includes function definitions,
it also has the same naming and structure as aliases:

```
functions/
    | <funnction name>
    | <funnction name>
    | <funnction name>
    | <funnction name>
```

Same rules for naming as aliases, commands and environments

It gets expanded like this:

```bash
function <function_name>() {
    <your code>
}
```

### plugin/keybinds

This directory includes [GNU readline syntax](https://www.gnu.org/software/bash/manual/html_node/Readline-Init-File-Syntax.html)
files, they will get `bind`ed

Folder structure should look like this:

```
keybinds/
    | all.rl
    | bindings/
        | <context name>
        | <context name>
        | <context name>
        | <context name>
```

- `all.rl`

This includes full-context bindings, it will
work in any context (keymap)

- `bindings`

This directory includes files which again files
with readline syntax but they get binded to a specific
context, for example `vi-command`, `emacs-meta`, etc.
the context should be the name of the file

`all.rl` Basically expands to:

```bash
bind -f 'all.rl"
```

`bindings/...` Expands to:

```bash
bind -m '<context name>' -f '<context name (as path)>'
```

### plugin/runners

This directory includes files that will be sourced/ran
on load:

```
runners/
    | <runner name>
    | <runner name>
    | <runner name>
    | <runner name>
```

It gets expanded like this:

```bash
source <runner name>
```
