#include "customCommands.h"
#include "globalError.h"

#include <string.h>
#include <stdio.h> //dbg
#include <stdlib.h>
#include <err.h>

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
    exit(errorCode);
}

void handleCd(char** args)
{
    char* dir = args[1];
    if (dir == NULL)
    {
        dir = getenv("HOME");
    }
    else if (strcmp(dir, "-") == 0)
    {
        dir = getenv("OLDPWD");
    }

    char* cwd = getenv("PWD");
    if (dir == NULL || cwd == NULL)
    {
        err(1, "cd failed\n");
    }

    if (setenv("OLDPWD", cwd, 1) != 0)
    {
        err(1, "cd failed\n");
    }

    if (setenv("PWD", dir, 1) != 0)
    {
        err(1, "cd failed\n");
    }
}
