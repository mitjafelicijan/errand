**This project is under active development and still considered work in
progress.**

# What is errand?

Errand is simple task runner that serves as an alternative to GNU Make. But
instead of using it as a build system Errand should only be used for running
tasks where simple build systems can be part of a task.

## Some facts about the project

- By default shells are pure so if you would want to have access to lets say
  `.bashrc` you would need to source it inside of a task. This way the task
  don't rely on personal settings and therefor are more likely to be
  reproducible on other machines.
- All of the contents of a task gets ran in one shell so you don't need `\` like
  you do in Make. You can also reuse outputs of previous lines. The tasks are
  behaving more like a shell script.
- By default all environmental variables do not get passed to Errand but you can
  enable this by setting `@env on` at the beginning of the `Errandfile`.
  
## How this all works?

1. In your project folder create file `Errandfile` and put the following code in
   it. Be default `erd` expects `Errandfile` to be there. You can override which
   file will be used by `-f` flag (`erd -f otherfile`).
2. Then run `erd` command in this folder. Just like with `make`.

   ```ruby
   @env on
   
   @task demo "A job doing something"
     # Shell commands are executed in the current directory
     ls -l
     pwd
   end
   ```
3. If no arguments were provided `erd` will display list of all available tasks
   in the `Errandfile`.
4. If you want to run a specific task then run `erd demo` and this will execute
   appropriate task. You can also chain tasks with `erd demo othertask` etc. The
   program will follow order of you arguments and try to execute all tasks.

## Tested with

- glibc GCC 12 (gcc)
- glibc Clang 11 (clang)
- glibc TinyCC 2022-07-15 (tcc)
- musl GCC 12 (musl-gcc)
- musl Clang 11 (musl-clang)

## Compilation

Clone the repository and do `make`. This will use the default compiler provided
with `$CC`.

You can also provide additional flags to C compiler with `EXTRA_FLAGS`
variable. For static compilation this would looks like `make
EXTRA_FLAGS=-static`. If you want to include debug symbols you could do it with
`EXTRA_FLAGS=-static -ggdb`. You get the gist.

Here are couple of examples.

```sh
# Dynamic compilation with gcc
make clean && CC=gcc make

# Static compilation with clang
make clean && CC=clang make EXTRA_FLAGS=-static

# Static compilation with gcc
make clean && CC=gcc make EXTRA_FLAGS=-static

# Static compilation with musl clang
make clean && CC=musl-clang make EXTRA_FLAGS=-static

# Dynamic compilation with tinycc
make clean && CC=tcc make
```

Alternatively you can use Nix shell which already provides three compilers (GCC,
Clang, TinyCC). To invoke the shell do `nix-shell`.

## Caveats & recommendations

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

## Features, ideas & todo's

### Implemented

- Override default shell with `@shell /bin/bash` operation. For the first
  version this should be in global scope.

### Would want to have

- Parallel execution (similar to `make -jN`). This should be provided with a
  flag `erd -p` and should automatically detect number tasks user wants to
  execute and then execute those tasks in parallel with pthreads.
- Read environmental variables and toggle that option with `@env [on/off]`.
- Check for existence of commands on a machine with `@assure gcc valgrind` where
  this will execute `command -v [cmd]` to check for existence of the program on
  a local machine.
- Allow creating of environmental variables in the global scope with `@var
  SOMEVAR "somevalue"`. These variable should be passed to all tasks and if
  `@env on` then these variables should override existing ones or be appended to
  the list.

## License

[erd](https://github.com/mitjafelicijan/errand) was written by [Mitja
Felicijan](https://mitjafelicijan.com) and is released under the BSD two-clause
license, see the LICENSE file for more information.
