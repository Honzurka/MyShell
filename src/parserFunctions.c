#include "parserFunctions.h"
#include "globalError.h"
#include "customCommands.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h> //dbg
#include <unistd.h>
#include <libgen.h>
#include <err.h>
#include <sys/wait.h>

extern char** environ;

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
        err(1, "malloc failed\n");
    }

    sprintf(result, "%s %s", str1, str2);

    free(str1);
    free(str2);

    return result;
}

int countCharOccurencesInStr(char c, char* str)
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
    int argCount = 2; //1 for name, 1 for last NULL
    if (args != NULL)
    {
        argCount += countCharOccurencesInStr(' ', args) + 1;
    }

    char** result = malloc(argCount * sizeof(char*));
    if (result == NULL)
    {
        err(1, "malloc failed\n");
    }

    result[0] = name;
    result[argCount - 1] = NULL;

    if (args != NULL)
    {
        size_t i = 1;
        char* token = strtok(args, " ");
        while (token != NULL)
        {
            result[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
    }

    return result;
}

/*
* returns absolute path to program
* or NULL if not found
*/
char* findProgramInPATH(char* progName)
{
    char* result = NULL;

    char* path = getenv("PATH");
    if (path == NULL)
    {
        err(1, "PATH not set\n");
    }

    char* pathCopy = strdup(path);
    if (pathCopy == NULL)
    {
        err(1, "strdup failed\n");
    }

    char* token = strtok(pathCopy, ":");
    while (token != NULL)
    {
        char progPath[strlen(token) + strlen(progName) + 2];
        sprintf(progPath, "%s/%s", token, progName);
        if (access(progPath, F_OK) == 0)
        {
            result = strdup(progPath);
            break;
        }
        token = strtok(NULL, ":");
    }

    free(pathCopy);
    return result;
}

void waitForChild()
{
    int status;
    wait(&status);
    handleChildStatus(status);
}

void handleCommand(char* path, char* args)
{
    printf("dbg: running command: %s with args %s\n", path, args);

    char* pathCopy = strdup(path);
    if (pathCopy == NULL)
    {
        err(1, "strdup failed\n");
    }

    char* name = basename(pathCopy);
    if (strcmp(path, name) == 0)
    {
        free(path);
        path = findProgramInPATH(name);
    }

    char** argArr = createArgArray(name, args);

    pid_t pid = fork();
    switch (pid) {
        case -1:
            err(1, "fork failed\n");
            break;
        case 0: //child
            execv(path, argArr);
            break;
        default: //parent
            waitForChild();
            break;
    }

    free(path);
    free(pathCopy);
    free(args);
    free(argArr);
}

void runCommand(char* path, char* args)
{
    int customCommandID = getCustomCommandID(path);
    if (customCommandID != -1)
    {
        char** argArr = createArgArray(path, args);
        handleCustomCommand(customCommandID, argArr);
        free(path);
        free(args);
    }
    else
    {
        handleCommand(path, args);
    }
}