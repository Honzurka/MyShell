/*
* Only 1 error per line is reported - same behavior as bash
*/

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
#define UNKNOWN_COMMAND_ERR 127


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
                err(UNKNOWN_COMMAND_ERR, "Unknown option\n");
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

void processLine(char* line)
{
    resetError();

    YY_BUFFER_STATE buf = yy_scan_string(line);
    yyparse(); // error code is handled through globalError
    yy_delete_buffer(buf);
}

void updatePrompt(char* prompt)
{
    memset(prompt, 0, MAXNAMLEN);
    char* cwd = getenv("PWD"); 
    if (cwd == NULL) {
        err(1, "PWD not set.\n");
    }
    strcat(prompt, "[MY_SHELL]");
    strcat(prompt, cwd);
    strcat(prompt, ": ");
}

void processInteractive()
{
    char prompt[MAXNAMLEN] = "";
    char* line;
    while(1) {
        updatePrompt(prompt);
        line = readline(prompt);

        processLine(line);
        if (errorCode != 0)
        {
            reportError();
        }

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
        err(UNKNOWN_COMMAND_ERR, "Unable to open script file: %s", fileName);
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, file)) != -1) {
        processLine(line);
        if (errorCode != 0)
        {
            reportError();
            exit(errorCode);
        }
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
    if (errorCode != 0)
    {
        reportError();
    }
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
            err(UNKNOWN_COMMAND_ERR, "Switch on unknown cmd source.");
    }

    return errorCode;
}