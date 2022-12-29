# Compile & run
1. clone repository
2. pull submodules: `git submodule update --init --recursive`
3. compile: `make`
4. run
    - interactive `./mysh`
    - command `./mysh -c SHELL_COMMANDS`
    - script `./mysh SCRIPT_FILE`

# Run valgrind
- `make run-valgrind`

# Run tests
1. compile
2. cd to `tests`
3. run tests
    - phase 1: `./run-tests.sh $(cat phase-1.tests)`
    - all: `./run-tests.sh` 
