# TODOs
- fix test36
- add tests as submodule?
- run valgrind on tests

# General
- [x] create makefile
    - -Wall, -Wextra 
- [x] check c-style
- [x] test on lab machine
- [ ] checks with valgrind
- [ ] read extraFeedback + web <--------------------------

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
- **DONE** `^C = SIGINT` clears line (wont kill shell)
    - but `kills child` process
- **DONE** empty cmd => new prompt
- **DONE** command execution
    - `cmd args`
    - separation by `;`
- **DONE** `^D/exit` to exit shell
    - shouldn't change `retval` from previous execution
- **DONE** `cd` - must set `$PWD, $OLDPWD`
    - `cd` == HOME
    - `cd -` go to previous dir (its enough to remember 2 (PWD+OLDPWD))
    - `cd dir`
- **DONE**`-c option`: execute cmd and exit
- **DONE** support for `cmd #comment`
- **DONE** non-interactive mode: `mysh test.sh`
    - exit on first error
        - prints lineNum + errormsg
- **DONE** retval == retval of last command
    - 127 == unknown command
    - 128+sigNum `killed by sigNum`- if child process is signalled
- **DONE** errors+warnings -> `stderr`
- **DONE** `cwd in prompt`

## Phase 2
- **DONE** pipes <----------- use linked list to represent commands (after that cascade-free whole list)
    - shell is parent of all processes
    - shell waits for all commands in pipeline to finish before next prompt
- **DONE** redirections
    - only std `>(out, truncate),<(in),>>(out, append)`
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