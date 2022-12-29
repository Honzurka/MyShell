#include "customCommands.h"
#include "globalError.h"
#include "helpers.h"

#include <errno.h>
#include <libgen.h>
#include <string.h>
#include <stdio.h> //dbg
#include <stdlib.h>
#include <err.h>
#include <unistd.h>

typedef void (*CommandHandler)(char** args);
CommandHandler customHandlers[] = {
    handleExit,
    handleCd
};

char* customCommands[] = {
    "exit",
    "cd"
};


int getCustomCommandID(char* command)
{
    for (int i = 0; i < sizeof(customCommands) / sizeof(customCommands[0]); i++)
    {
        if (strcmp(command, customCommands[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

void handleCustomCommand(int commandID, char** args)
{
    customHandlers[commandID](args);
    free(args);
}

void handleExit(char** args)
{
    //MAYBE TODO: handle `exit argVal`
    exit(errorCode);
}

void handleCd(char** args)
{
    int argCount = getArgCount(args);
    if (argCount > 2)
    {
        char* msg = allocateString("cd: too many arguments");
        setError(GENERAL_ERROR, msg, 0);
        return;
    }

    char* dir = args[1];
    if (dir == NULL)
    {
        dir = getenv("HOME");
    }
    else if (strcmp(dir, "-") == 0)
    {
        dir = getenv("OLDPWD");

        char* dirCopy = strdup(dir);
        if (dirCopy[strlen(dirCopy) - 1] == '/')
        {
            dirCopy[strlen(dirCopy) - 1] = '\0';
        }
        printf("%s\n", dirCopy);
        free(dirCopy);
    }

    char* cwd = getenv("PWD");
    if (cwd == NULL || dir == NULL)
    {
        err(GENERAL_ERROR, "cd failed\n");
    }

    if (chdir(dir) != 0)
    {
        char* msg = allocateString(strerror(errno));
        setError(GENERAL_ERROR, msg, 0);
        return;
    }

    if (setenv("OLDPWD", cwd, 1) != 0)
    {
        err(GENERAL_ERROR, "cd failed\n");
    }
    if (setenv("PWD", dir, 1) != 0)
    {
        err(GENERAL_ERROR, "cd failed\n");
    }
}
