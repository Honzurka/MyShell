#include "parserFunctions.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h> //dbg
#include <unistd.h>
#include <libgen.h>
#include <err.h>

/*
* Concatenates two strings, freeing the originals
*/
char* concatArgs(char* str1, char* str2)
{
    int len = 0;
    len += strlen(str1);
    len += strlen(str2);

    char* result = malloc(len + 2);
    if (result == NULL)
    {
        err(1, "malloc failed");
    }

    strcat(result, str1);
    strcat(result, " ");
    strcat(result, str2);

    free(str1);
    free(str2);

    return result;
}

int charOccurencesInStr(char c, char* str)
{
    int result = 0;
    for (char* str_p = str; *str_p != '\0'; str_p++) {
        if (*str_p == c) {
            result++;
        }
    }
    return result;
}

/*
* Creates argument array for execv
* from space separated args string
*/
char** createArgArray(char* name, char* args)
{
    int argCount = charOccurencesInStr(' ', args) + 3; // +1 for name, +1 for last string, +1 for last NULL
    char** result = malloc(argCount * sizeof(char*));
    if (result == NULL)
    {
        err(1, "malloc failed");
    }

    result[0] = name;
    result[argCount - 1] = NULL;
    size_t i = 1;
    char* token = strtok(args, " ");
    while (token != NULL)
    {
        result[i] = token;
        i++;
        token = strtok(NULL, " ");
    }

    return result;
}

void runCommand(char* path, char* args)
{
    printf("dbg: running command: %s with args %s\n", path, args);

    char* pathCopy = strdup(path);
    if (pathCopy == NULL)
    {
        err(1, "strdup failed");
    }

    char* name = basename(pathCopy);
    if (strcmp(path, name) == 0)
    {
        //search for name in PATH => update path
    }

    char** argArr = createArgArray(name, args);

    // todo: fork
    execv(path, argArr);
    printf("finished execv\n");

    free(path);
    free(pathCopy);
    free(args);
    free(argArr);
}