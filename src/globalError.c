#include "globalError.h"
#include "helpers.h"
#include <stdlib.h>
#include <stdio.h>

int errorCode = 0;
char* errorMessage = NULL;
int errorLine = 0;

/*
* Provided `message` must be allocated on heap
*/
void setError(int code, char* message, int line)
{
    free(errorMessage);

    errorCode = code;
    errorMessage = message;
    errorLine = line;
}

void setErrorWithAlloc(int code, char* message, int line)
{
    char* msg = allocateString(message);
    setError(code, msg, line);
}

void resetError()
{
    free(errorMessage);

    errorCode = 0;
    errorMessage = NULL;
    errorLine = 0;
}

void reportError()
{
    switch (errorCode) {
        case SYNTAX_ERROR:
            fprintf(stderr, "error:%d %s\n", errorLine, errorMessage);
            break;
        case GENERAL_ERROR:
            fprintf(stderr, "error: %s\n", errorMessage);
            break;
        case UNKNOWN_COMMAND_ERROR:
            fprintf(stderr, "unknown command: %s\n", errorMessage);
            break;
        default:
            break;
    }
}

void handleChildStatus(int status) //MAYBE TODO: fix line numbers --- probably has to be done in parser---------------------------------
{
    if (WIFEXITED(status))
    {
        setErrorWithAlloc(WEXITSTATUS(status), "Child process exited", 0);
    }

    if (WIFSIGNALED(status))
    {
        setErrorWithAlloc(SIGNAL_BASE + WTERMSIG(status), "Child process was signaled", 0);
        //TODO?: WCOREDUMP
        //TODO?: WSTOPSIG+WIFCONTINUED
    }
}
