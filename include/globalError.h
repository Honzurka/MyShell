#ifndef GLOBAL_ERROR_H
#define GLOBAL_ERROR_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define SYNTAX_ERROR 2

extern int errorCode;
extern char* errorMessage;
extern int errorLine;

static inline void setError(int code, char* message, int line)
{
    free(errorMessage);

    errorCode = code;
    errorMessage = message;
    errorLine = line;
}

static inline void resetError()
{
    free(errorMessage);

    errorCode = 0;
    errorMessage = NULL;
    errorLine = 0;
}

static inline void reportError()
{
    fprintf(stderr, "error:%d %s\n", errorLine, errorMessage);
}

#endif