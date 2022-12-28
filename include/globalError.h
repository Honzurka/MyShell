#ifndef GLOBAL_ERROR_H
#define GLOBAL_ERROR_H

#define SYNTAX_ERROR 2
#define SIGNAL_BASE 128

extern int errorCode;
extern char* errorMessage;
extern int errorLine;

void setError(int code, char* message, int line);
void resetError();
void reportError();
void handleChildStatus(int status);

#endif