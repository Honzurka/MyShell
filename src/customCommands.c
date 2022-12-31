#include "customCommands.h"
#include "globalError.h"
#include "helpers.h"
#include <err.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef void (*CommandHandler)(char** args);
CommandHandler customHandlers[] = {handleExit, handleCd};

char* customCommands[] = {"exit", "cd"};

int getCustomCommandID(char* command) {
    for (int i = 0;
         i < (int)(sizeof(customCommands) / sizeof(customCommands[0])); i++) {
        if (strcmp(command, customCommands[i]) == 0) {
            return i;
        }
    }
    return -1;
}

/*
 * handlers must reset error before calling command (due to custom exit command)
 */
void handleCustomCommand(int commandID, char** args) {
    customHandlers[commandID](args);
    free(args);
}

void handleExit(char** args) {
    (void)args;
    // MAYBE TODO: handle `exit argVal`
    exit(errorCode);
}

void handleCd(char** args) {
    resetError();
    int argCount = getArgCount(args);
    if (argCount > 2) {
        setErrorWithAlloc(GENERAL_ERROR, "cd: too many arguments", 0);
        return;
    }

    char* dir = args[1];
    if (dir == NULL)   // no arg
    {
        dir = getenv("HOME");
        if (dir == NULL) {
            setErrorWithAlloc(GENERAL_ERROR, "HOME not set", 0);
            return;
        }
    } else if (strcmp(dir, "-") == 0)   // arg is '-'
    {
        dir = getenv("OLDPWD");
        if (dir == NULL) {
            setErrorWithAlloc(GENERAL_ERROR, "OLDPWD not set", 0);
            return;
        }

        char* dirCopy = strdup(dir);
        if (dirCopy[strlen(dirCopy) - 1] == '/') {
            dirCopy[strlen(dirCopy) - 1] = '\0';
        }
        printf("%s\n", dirCopy);
        free(dirCopy);
    }

    int replaceOLDPWD = 1;
    char* cwd = getenv("PWD");
    if (cwd == NULL) {
        setErrorWithAlloc(GENERAL_ERROR, "PWD not set", 0);
        cwd = "";
        replaceOLDPWD = 0;
    }

    // change dir and env vars
    if (chdir(dir) != 0) {
        setErrorWithAlloc(GENERAL_ERROR, strerror(errno), 0);
        return;
    }
    if (setenv("OLDPWD", cwd, replaceOLDPWD) != 0) {
        err(GENERAL_ERROR, "Unable to set OLDPWD\n");
    }
    if (setenv("PWD", dir, 1) != 0) {
        err(GENERAL_ERROR, "Unable to set PWD\n");
    }
}
