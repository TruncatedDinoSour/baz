# sanitization

> i hate regex, its such a pain
>
> - Me, developing baz_sanitize function

basically, this sanitises stuff,
this is what is not allowed

- any whitespace
- characters : ", ', ~, !, #, \, /, $, %, ^, &, \*, (, ), =
- repeating `..`s

regex ( sed syntax ):

```sed
s/\s*//g; s/[\"\'~\!#\\\/\$%\^&\*\(\)\=]//g; s/\.\.//g
```
