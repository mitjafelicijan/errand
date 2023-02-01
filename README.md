# Errand - A simple task runner for the command line

Errand is a simple task runner for the command line. It is written in Go and is designed to be simple and easy to use.

More coming soon...

## Simple example

```ruby
@task demo "A job doing something" does
  # Shell commands are executed in the current directory
  ls -l
  pwd
end
```

## Installation

All the pre-built binaries are available on [https://files.mitjafelicijan.com/errand/](https://files.mitjafelicijan.com/errand/).

Download the binary for your platform and put it in your PATH.

## Testing Errand with test tasks

```sh
ERRANDFILE=test-data/Errandfile ./erd
```