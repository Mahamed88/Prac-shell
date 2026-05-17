# Prac-Shell

A Unix-like shell written in C that supports command execution, built-in commands, pipelines, I/O redirection, signal handling, and both interactive and batch execution modes.

---

# Features

## Core Shell Features

* Interactive shell mode
* Batch file execution mode
* Multiple commands separated by semicolons (`;`)
* External command execution using `fork()` and `execvp()`
* Process synchronization using `waitpid()`

## Built-In Commands

* `cd` – change working directory
* `exit` – terminate the shell
* `path` – display, add, and remove PATH directories
* `myhistory` – maintain and display shell command history
* `alias` – create custom command aliases

## Advanced Features

* Input redirection (`<`)
* Output redirection (`>`)
* Command pipelines (`|`)
* Signal handling for `Ctrl-C` and `Ctrl-Z`
* PATH environment variable management
* Error handling and defensive programming

---

# Technologies Used

* C Programming Language
* POSIX System Calls
* Linux/macOS Terminal Environment
* Git & GitLab Collaboration

System calls and APIs used include:

* `fork()`
* `execvp()`
* `waitpid()`
* `pipe()`
* `dup2()`
* `open()`
* `signal()`
* `setenv()`
* `chdir()`

---

# Project Structure

```text
Prac-Shell/
│
├── major.c                 # Main program entry point
├── shell.c                 # Shell engine and command execution
├── shell.h                 # Shell definitions and declarations
├── builtins.c              # Built-in command dispatcher
├── builtins.h              # Built-in function declarations
├── builtin_cd.c            # cd implementation
├── builtin_exit.c          # exit implementation
├── builtin_path.c          # path implementation
├── builtin_myhistory.c     # command history implementation
├── builtin_alias.c         # alias implementation
├── makefile                # Build instructions
└── README.md               # Project documentation
```

---

# Compilation

Compile the project using GCC:

```bash
gcc *.c -o newshell
```

Or using make:

```bash
make
```

---

# Running the Shell

## Interactive Mode

```bash
./newshell
```

Example:

```text
newshell> ls
newshell> pwd
newshell> path
```

## Batch Mode

```bash
./newshell batch.txt
```

Where `batch.txt` contains commands such as:

```text
ls
pwd
path
```

---

# Example Commands

## PATH Management

```text
newshell> path
newshell> path + /tmp
newshell> path - /tmp
```

## Multiple Commands

```text
newshell> pwd ; ls ; path
```

## Pipelining

```text
newshell> ls | wc
```

## Redirection

```text
newshell> ls > output.txt
newshell> sort < input.txt
```

## Aliases

```text
newshell> alias ll='ls -la'
newshell> ll
```

## Command History

```text
newshell> myhistory
```

---
# Demonstration Screenshots

## 1. PATH Management

Demonstrates the custom `path` built-in command for adding and removing directories from the shell PATH variable.

Command used:

```text
newshell> path + /tmp ; path ; path - /tmp ; path
```

<img width="1200" alt="PATH Management Demo" src="screenshots/path-management.png">

---

## 2. Input and Output Redirection

Demonstrates support for output redirection (`>`) and input redirection (`<`).

Command used:

```text
newshell> echo "hello shell" > demo.txt ; cat < demo.txt ; rm demo.txt
```

<img width="1200" alt="Redirection Demo" src="screenshots/redirection-demo.png">

---

## 3. Alias Support and Command History

Demonstrates alias creation and command history tracking using `myhistory`.

Command used:

```text
newshell> alias ll="ls -l" ; ll ; myhistory
```

<img width="1200" alt="Alias and History Demo" src="screenshots/history-alias-demo.png">

---

# Design Overview

The shell is organized into modular components separating the shell engine from built-in command functionality.

* `major.c` manages program startup, interactive mode, and batch mode.
* `shell.c` handles parsing, command execution, pipelines, and redirection.
* `builtins.c` routes built-in commands to their implementations.
* Individual built-in commands are implemented in separate files for modularity and team collaboration.

The shell uses POSIX process management and inter-process communication mechanisms to execute commands and support shell functionality.

---

# Error Handling

The shell includes defensive programming techniques to handle:

* Invalid commands
* Missing files
* Incorrect batch file usage
* Excessively long input lines
* Redirection failures
* Pipe creation failures
* Forking failures

The shell continues executing whenever possible instead of terminating unexpectedly.

---

# Future Improvements

* Background process execution

* Advanced job control

* Tab completion

* Environment variable expansion

* Recursive alias expansion

* Improved parsing for quoted strings

* Background process execution

* Advanced job control

* Tab completion

* Environment variable expansion

* Recursive alias expansion

* Improved parsing for quoted strings

---

# License

This project was created for educational and systems programming practice purposes.
