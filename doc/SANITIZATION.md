# Sanitization

> I hate regex, it's such a pain
>
> - Me, developing baz_sanitize function

Basically, this sanitises a lot,
this is what is not allowed

- Any whitespace
- Characters: ", ', ~, !, #, \, /, $, %, ^, &, \*, (, ), =
- Repeating `..`s

Regex (sed syntax):

```sed
s/\s*//g; s/[\"\'~\!#\\\/\$%\^&\*\(\)\=]//g; s/\.\.//g
```
