#ifndef GLOBAL_ERROR_H
#define GLOBAL_ERROR_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define SYNTAX_ERROR 2

extern int errorCode;
extern char* errorMessage;
extern int errorLine;

void setError(int code, char* message, int line);
void resetError();
void reportError();
void exitWithErrorCode();

#endif