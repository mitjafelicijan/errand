# Errand - A simple task runner for the command line

Errand is a simple task runner for the command line. It is written in Go and is designed to be simple and easy to use.

More coming soon...

## Simple example

```ruby
@task stack "Starts Docker stack" does
  # Shell commands are executed in the current directory
  ls -l
  pwd
end
```

## Testing Errand with test tasks

```sh
ERRANDFILE=test-data/Errandfile ./erd
```