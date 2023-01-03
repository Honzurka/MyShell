#ifndef GLOBAL_ERROR_H
#define GLOBAL_ERROR_H

#define JUST_WARNING 0
#define GENERAL_ERROR 1
#define SYNTAX_ERROR 2
#define UNKNOWN_COMMAND_ERROR 127
#define SIGNAL_BASE 128

extern int errorCode;
extern char* errorMessage;
extern int errorLine;

void setError(int code, char* message, int line);
void setErrorWithAlloc(int code, char* message, int line);
void resetError();
void reportError();
void handleChildStatus(int status);

#endif