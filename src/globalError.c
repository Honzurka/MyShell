#include "globalError.h"

int errorCode = 0;
char* errorMessage = NULL;
int errorLine = 0;

void setError(int code, char* message, int line)
{
    free(errorMessage);

    errorCode = code;
    errorMessage = message;
    errorLine = line;
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
    fprintf(stderr, "error:%d %s\n", errorLine, errorMessage);
}

void exitWithErrorCode()
{
    exit(errorCode);
}