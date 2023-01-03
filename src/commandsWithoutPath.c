#include "commandsWithoutPath.h"
#include "globalError.h"
#include <linux/limits.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

typedef void (*CommandHandler)(char** args);
CommandHandler commandHandlers[] = {handlePwd};

char* knownCommands[] = {"pwd"};

int getCommandID(char* command) {
    for (int i = 0; i < (int)(sizeof(knownCommands) / sizeof(knownCommands[0]));
         i++) {
        if (strcmp(command, knownCommands[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int tryExecutionWithoutPath(char* name, char** args) {
    int idx = getCommandID(name);
    if (idx == -1) {
        return 0;
    }

    commandHandlers[idx](args);

    return 1;
}

void handlePwd(char** args) {
    (void)args;
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        setErrorWithAlloc(GENERAL_ERROR, "getcwd failed", 0);
    }
}