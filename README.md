# CLI Project

A custom command-line shell implemented in C++. It features a read-eval-print loop (REPL) with a customizable prompt, I/O redirection, command piping, and a set of built-in commands.

## Architecture

Input is processed through a three-stage pipeline:

```
User Input → Tokenizer → Parser → Executor → Output
```

| Component | File | Responsibility |
|-----------|------|----------------|
| `Console` | `Console.cpp` | Singleton REPL loop, manages prompt symbol |
| `Tokenizer` | `Tokenizer.cpp` | Lexes raw input into tokens (strings, pipes, redirects) |
| `Parser` | `Parser.cpp` | Transforms token stream into `CommandNode` objects |
| `Executor` | `Executor.cpp` | Dispatches commands, handles piping and I/O redirection |
| `Command` | `Command.cpp` | Implementations of all built-in commands |
| `DataTypes` | `DataTypes.h` | Shared data structures (`Token`, `CommandNode`, `Redirect`) |

## Built-in Commands

| Command | Syntax | Description |
|---------|--------|-------------|
| `echo` | `echo ["text"\|file]` | Print a quoted string, file contents, or stdin to stdout |
| `prompt` | `prompt "symbol"` | Change the shell prompt symbol |
| `time` | `time` | Print the current time (`HH:MM:SS`) |
| `date` | `date` | Print the current date (`YYYY-MM-DD`) |
| `touch` | `touch <file>` | Create a new empty file |
| `truncate` | `truncate <file>` | Empty an existing file |
| `rm` | `rm <file>` | Delete a file |
| `wc` | `wc -w\|-c [file\|"text"]` | Count words (`-w`) or characters (`-c`) |
| `tr` | `tr [file\|"text"] -"what" ["with"]` | Replace or remove occurrences of a substring |
| `head` | `head -n<count> [file\|"text"]` | Print the first N lines |
| `batch` | `batch <file>` | Execute commands from a script file line by line |

## Shell Features

### I/O Redirection

```sh
echo input.txt > out.txt        # redirect stdout to a file (overwrite)
echo input.txt >> out.txt       # redirect stdout to a file (append)
wc -w < input.txt               # redirect file to stdin
```

### Piping

Chain commands together with `|`. The stdout of each command feeds into the stdin of the next:

```sh
echo input.txt | wc -w
echo input.txt | tr -"hello" "world" | wc -c
```

### Quoted Strings

Both single and double quotes are supported for strings containing spaces:

```sh
echo "Hello, World!"
tr "Hello World" -"World" "CLI"
```

### Batch Scripting

Run a sequence of commands stored in a plain text file:

```sh
batch commands.txt
```

Each line in the file is executed as a separate command, supporting all shell features including pipes and redirections.

## Platform

Windows only — the project depends on `windows.h` for console cursor detection used to manage newline output.
