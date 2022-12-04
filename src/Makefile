CC = clang
CFLAGS = -Wall -Wextra -std=c99 -lfl
OBJECTS = mysh.o lex.yy.o parser.tab.o
EXEC = mysh

all: parser scanner $(EXEC)

parser: parser.y
	bison -d parser.y

scanner: parser scanner.l
	flex scanner.l

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $(EXEC)

clean:
	rm lex.yy.c scanner.h parser.tab.c parser.tab.h
	rm $(OBJECTS)

# objects
mysh.o: mysh.c
	$(CC) -c mysh.c
lex.yy.o: lex.yy.c
	$(CC) -c lex.yy.c
parser.tab.o: parser.tab.c
	$(CC) -c parser.tab.c