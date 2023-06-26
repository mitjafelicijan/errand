# errand

**This project is under active development and still considered work in
progress.**

Errand is simple task runner that serves as an alternative to GNU Make. But
instead of using it as a build system Errand should only be used for simple
tasks.

## Simple example

```ruby
@task demo "A job doing something"
  # Shell commands are executed in the current directory
  ls -l
  pwd
end
```

If the folder contains `Errandfile` then you can use `erd` command to start
using and executing tasks

## Tested with

- GCC 12
- Clang 11
- TinyCC 2022-07-15

## Compile and try

Clone the repository and do `make`. This will use the default compiler provided
with `$CC`.

Alternatively you can use Nix shell which already provides three compilers (GCC,
Clang, TinyCC). To invoke the shell do `nix-shell`.

```sh
make clean && CC=clang make && ./erd
make clean && CC=gcc make && ./erd
make clean && CC=tcc make && ./erd
```
