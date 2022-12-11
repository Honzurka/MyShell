# General
- [x] create makefile
    - -Wall, -Wextra 
- [ ] check c-style
- [ ] test on lab machine
- [ ] checks with valgrind

## Implementation
- `readline` to get input
- `flex & bison` for cmd parsing
- `<queue.h>` linked list implementation `man queue`
    - `STAIL_*` and `TAIL_*` macros
- prohibited functions
    - fopen, fread => open, read
    - popen, system => fork

## Phase 1
- **DONE** use existing `path` from parent shell
- `^C = SIGINT` clears line (wont kill shell)
    - but `kills child` process
- **DONE** empty cmd => new prompt
- **DONE** command execution
    - `cmd args`
    - separation by `;`
- `^D/exit` to exit shell
    - shouldn't change `retval` from previous execution
- `cd` - must set `$PWD, $OLDPWD`
    - `cd` == HOME
    - `cd -` go to previous dir (its enough to remember 2 (PWD+OLDPWD))
    - `cd dir`
- **DONE**`-c option`: execute cmd and exit
- support for `cmd #comment`
- non-interactive mode: `mysh test.sh`
    - exit on first error
        - prints lineNum + errormsg
- retval == retval of last command
    - 127 == unknown command
    - 128+sigNum `killed by sigNum`- if child process is signalled
- errors+warnings -> `stderr`
- **DONE** `cwd in prompt`

## Phase 2
- pipes <----------- use linked list to represent commands (after that cascade free whole list)
    - shell is parent of all processes
    - shell waits for all commands in pipeline to finish
- redirections
    - only std `>,<,>>`
    - anywhere on line
        - (if multiple: `> & >>`)   last one is applied
    - combination with pipeline: redirection is applied after each command


# flex (scanning)
- 3 sections
    - declarations
    - patterns + actions
    - C-code
- match preference: longer, first written
- `"char"` - raw char == not matched
- can be used as coroutine => `yylex` returns next token
    - TOKEN `{ return ...}`
        - if return => yylex returns this token
        - otherwise => yylex continues scanning `eg. useful for whitespace`

# bison (parsing)
- tokens: `%token TOK`
- rule: `start_symbol: different_start_symbol TOK { ... }`
    - value: `$$ target symbol`, `$1 rule symbol` (read from yylval when scanner returns token)
    - `{}`

## parsing from string
- lex
    - with copy
        - `YY_BUFFER_STATE yy_scan_string(const char *str)`: scans a NUL-terminated string`
            - returns buf => deleted by `yy_delete_buffer()`
    - without copy
        - `YY_BUFFER_STATE yy_scan_buffer(char *base, yy_size_t size)`
- `yyparse()` triggers bison parsing (call this after buffer is set)
    - repeatedly calls `yylex()` to read token