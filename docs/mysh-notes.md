# General
- [ ] create makefile
    - -Wall, -Wextra 
- [ ] check c-style
- [ ] test on lab machine
- [ ] checks with valgrind

# Implementation
- `readline` to get input
- `flex & bison` for cmd parsing
- `<queue.h>` linked list implementation `man queue`
    - `STAIL_*` and `TAIL_*` macros
- prohibited functions
    - fopen, fread => open, read
    - popen, system => fork

# Phase 1
- use existing `path` from parent shell
- `^C = SIGINT` clears line (wont kill shell)
    - but `kills child` process
- empty cmd => new prompt
- command execution
    - `cmd args`
    - separation by `;`
- `^D/exit` to exit shell
    - shouldn't change `retval` from previous execution
- `cd` - must set `$PWD, $OLDPWD`
    - `cd` == HOME
    - `cd -` go to previous dir (its enough to remember 2 (PWD+OLDPWD))
    - `cd dir`
- `-c option`: execute cmd and exit
- support for `cmd #comment`
- non-interactive mode: `mysh test.sh`
    - exit on first error
        - prints lineNum + errormsg
- retval == retval of last command
    - 127 == unknown command
    - 128+sigNum `killed by sigNum`- if child process is signalled
- errors+warnings -> `stderr`
- `cwd in prompt`

# Phase 2
- pipes
    - shell is parent of all processes
    - shell waits for all commands in pipeline to finish
- redirections
    - only std `>,<,>>`
    - anywhere on line
        - (if multiple: `> & >>`)   last one is applied
    - combination with pipeline: redirection is applied after each command


