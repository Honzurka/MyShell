#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

// readline tutorial: https://web.mit.edu/gnu/doc/html/rlman_2.html
// compile: clang -lreadline readline.c && ./a.out 

int main(void)
{
	char* line;
	while(1) {
		line = readline("Enter command: ");

		printf("str2: %s\n", line);

		// if (line != NULL && *line != '\0') {
		if (line && *line) {
			add_history(line);
		}

		free(line);
	}
}