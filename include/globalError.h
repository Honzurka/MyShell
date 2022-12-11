#ifndef GLOBAL_ERROR_H
#define GLOBAL_ERROR_H

#include <stddef.h>

extern int errorCode;
extern char* errorMessage;

static inline void setError(int code, char* message) {
    errorCode = code;
    errorMessage = message;
}

#endif