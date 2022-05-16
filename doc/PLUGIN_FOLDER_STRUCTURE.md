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
