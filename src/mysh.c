#include "parser.tab.h"
#include "scanner.h"
#include "globalError.h"
#include <dirent.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_LINE_LEN 4096

typedef enum
{
    SCRIPT,
    COMMAND_STRING,
    INTERACTIVE
} commandSource_t;

commandSource_t getSource(int argc, char** argv, int* argStartIdx)
{
    commandSource_t result = INTERACTIVE;

    int c;
    while ((c = getopt(argc, argv, "c")) != -1)
    {
        switch (c)
        {
            case 'c':
                result = COMMAND_STRING;
                break;
            default:
                err(1, "Unknown option\n");
                break;
        }
    }

    if (argc - optind > 0) {
        if (result != COMMAND_STRING) {
            result = SCRIPT;
        }
        *argStartIdx = optind;
    }

    return result;
}

/*
* returns 0 if processing was successful
* otherwise returns 1 ----------------------------------- maybe set some global error flag?
*/
int processLine(char* line)
{
    // printf("dbg: processing line: %s\n", line);

    YY_BUFFER_STATE buf = yy_scan_string(line);
    int retval = yyparse();
    yy_delete_buffer(buf);

    return retval;
}

void processInteractive()
{
    // set prompt
    char* cwd = getenv("PWD"); 
    if (cwd == NULL) {
        err(1, "PWD not set.\n");
    }
    char prompt[MAXNAMLEN] = "";
    strcat(prompt, "[MY_SHELL]");
    strcat(prompt, cwd);
    strcat(prompt, ": ");

    // process input
    char* line;
    while(1) {
        line = readline(prompt);

        processLine(line);

        if (line && *line) {
            add_history(line);
        }

        free(line);
    }
}

void processScript(char** argv, int argvIdx)
{
    char* fileName = argv[argvIdx];
    FILE* file = fopen(fileName, "r");
    if (file == NULL)
    {
        fprintf(stderr, "can't open %s: %s\n", fileName, strerror(errno));
        err(1, "Unable to open script file.\n");
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, file)) != -1) {
        line[strcspn(line, "\n")] = 0; // strips newline
        processLine(line); //TODO: error handling-------------------------
    }

    free(line);
    fclose(file);
}

void processCommandString(int argc, char** argv, int argvIdx)
{
    char line[MAX_LINE_LEN] = {0};

    for (int i = argvIdx; i < argc; i++) {
        strcat(line, argv[i]);
        strcat(line, " ");
    }

    processLine(line);
}

int main(int argc, char** argv)
{
    int argvIdx = 0;
    commandSource_t src = getSource(argc, argv, &argvIdx);

    switch (src) {
        case INTERACTIVE:
            processInteractive();
            break;
        case SCRIPT:
            processScript(argv, argvIdx);
            break;
        case COMMAND_STRING:
            processCommandString(argc, argv, argvIdx);
            break;
        default:
            err(1, "Switch on unknown cmd source.");
    }

    return 0; // TODO---------
}