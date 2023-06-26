# errand

**This project is under active development and still considered work in
progress.**

Errand is simple task runner that serves as an alternative to GNU Make. But
instead of using it as a build system Errand should only be used for simple
tasks.

## Some facts about the project

- By default shells are pure so if you would want to have access to lets say
  `.bashrc` you would need to source it inside of a task. This way the task
  don't rely on personal settings and therefor are more likely to be
  reproducible on other machines.
- All of the contents of a task gets ran in one shell so you don't need `\` like
  you do in Make. You can also reuse outputs of previous lines. The tasks are
  behaving more like a shell script.
- By default all environmental variables get passed to Errand but you can
  disable this by setting `@env off` at the beginning of the `Errandfile`. This
  goes against reproducibility but the benefits outweigh the costs in this case.

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

## Caveats and recommendations

### Syntax highlighting

You can use Ruby language for syntax highlighting.

### Multi-line expressions are currently not provided.

Instead of this:
	
```sh
for ((i=1; i<=10; i++))
do
  echo $i
done
```
	
Do the following instead:

```sh
for i in {1..10}; do echo $i; done
```

Or make an external script and call it with `bash script.sh`.
