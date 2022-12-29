#ifndef GLOBAL_ERROR_H
#define GLOBAL_ERROR_H

#define GENERAL_ERROR 1 //TODO: replace in all files----------
#define SYNTAX_ERROR 2
#define UNKNOWN_COMMAND_ERROR 127
#define SIGNAL_BASE 128

extern int errorCode;
extern char* errorMessage;
extern int errorLine;

void setError(int code, char* message, int line);
void resetError();
void reportError();
void handleChildStatus(int status);

#endif