#include "customCommands.h"
#include "globalError.h"

#include <string.h>
#include <stdio.h> //dbg
#include <stdlib.h>


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
    printf("cd handler called\n"); //TODO-----------------
}
