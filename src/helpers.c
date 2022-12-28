#include "helpers.h"

#include <err.h>
#include <stdlib.h>
#include <string.h>

char* allocateString(char* str)
{
    char* result = malloc(strlen(str) + 1);
    if (result == NULL)
    {
        err(1, "malloc failed\n");
    }
    strcpy(result, str);
    return result;
}