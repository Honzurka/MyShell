#DBG: prvni 3 radky OK - bison build+flex part

# Name of the executable
EXE = mysh

# Compiler
CC = clang

# Bison and flex
BISON = bison
FLEX = flex

# Compiler flags
OBJECT_FLAGS = -I./include #-Wall -Wextra #maybe unnecessary - get rid of -Wall
CFLAGS = -I./include -Wall -Wextra -std=c99 -g -lfl -lreadline -g # -g for debugging

# List of source files
SRCS = $(wildcard src/*.c)

# List of bison and flex files
PARSERS = $(wildcard src/*.y)
LEXERS = $(wildcard src/*.l)

# List of object files
OBJS = $(PARSERS:.y=.tab.o) $(LEXERS:.l=.yy.o) $(SRCS:.c=.o) 

# Default target
all: $(EXE)

# Build the executable
$(EXE): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(EXE)

# Compile source files
%.o: %.c
	$(CC) $(OBJECT_FLAGS) -c $< -o $@

# Compile bison files
%.tab.c %.tab.h: %.y
	$(BISON) --header=$*.tab.h -o $*.tab.c $<

# Compile flex files
%.yy.c %.h: %.l
	$(FLEX) -o $@ --header-file=$*.h $<

# Clean up
clean:
	rm -f $(EXE) src/*.o src/*.tab.c src/*.tab.h src/*.yy.c src/scanner.h