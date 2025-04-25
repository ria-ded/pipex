<h1>Pipex</h1>
A simple Unix pipeline emulator written in C, for 42 School.

## 📌 About

__Pipex__ reproduces shell-like behavior for command pipelines like:

`<infile cmd1 | cmd2 >outfile`

It creates processes, handles file redirection, and mimics the output of the real shell using low-level system calls (fork, pipe, execve, etc.) without using any shell or high-level library functions.


## Usage

`make`
`./pipex infile "cmd1 args" "cmd2 args" outfile`

Example:
`make`
`./pipex input.txt "grep a" "wc -l" output.txt`


## Features

- Manual path resolution (PATH= parsing)
- Custom argument parsing
- Error handling with correct exit codes:
  - 127 for command not found
  - 126 for permission denied
- Graceful memory cleanup
- Robust pipe and file descriptor management
- Multi-command pipelines (cmd1 | cmd2 | cmd3 | ...)
  

## System Calls Used

`fork`
`pipe`
`dup2`
`execve`
`waitpid`
`open`
`close`


## File Structure

`pipex.c` – Entry point

`pipex_exec.c` – Pipe execution and command runs

`pipex_parse.c` – Argument and command parsing

`pipex_cleanup.c` – Memory management

`pipex_help.c` – Helpers (init, wrapper, wait)
