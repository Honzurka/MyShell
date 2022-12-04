CC = clang
CFLAGS = -Wall -Wextra -std=c99 -lfl -lreadline
OBJECTS = mysh.o lex.yy.o parser.tab.o
DEBUG = -g
EXEC = mysh

all: parser scanner $(EXEC)

parser: parser.y
	bison -d parser.y

scanner: parser scanner.l
	flex scanner.l

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) $(DEBUG) -o $(EXEC)

clean:
	rm -f lex.yy.c scanner.h parser.tab.c parser.tab.h $(OBJECTS)

# objects
mysh.o: mysh.c
	$(CC) $(DEBUG) -c mysh.c
lex.yy.o: lex.yy.c
	$(CC) $(DEBUG) -c lex.yy.c
parser.tab.o: parser.tab.c
	$(CC) $(DEBUG) -c parser.tab.c