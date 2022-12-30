#include "commandQueue.h"
#include "customCommands.h"
#include "helpers.h"
#include "globalError.h"
#include "parserFunctions.h"
#include <stdlib.h>
#include <stdio.h>   //dbg
#include <err.h>
#include <errno.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

/*
 * Creates argument array for execv
 * from space separated args string
 */
char** createArgArray(char* name, char* args) {
    int argCount = 2;   // 1 for name, 1 for last NULL
    if (args != NULL) {
        argCount += countCharOccurencesInStr(' ', args) + 1;
    }

    char** result = malloc(argCount * sizeof(char*));
    if (result == NULL) {
        err(1, "malloc failed\n");
    }

    result[0] = name;
    result[argCount - 1] = NULL;

    if (args != NULL) {
        size_t i = 1;
        char* token = strtok(args, " ");
        while (token != NULL) {
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
char* findProgramInPATH(char* progName) {
    char* result = NULL;

    char* path = getenv("PATH");
    if (path == NULL) {
        err(1, "PATH not set\n");
    }

    char* pathCopy = strdup(path);
    if (pathCopy == NULL) {
        err(1, "strdup failed\n");
    }

    char* token = strtok(pathCopy, ":");
    while (token != NULL) {
        char progPath[strlen(token) + strlen(progName) + 2];
        sprintf(progPath, "%s/%s", token, progName);
        if (access(progPath, F_OK) == 0) {
            result = strdup(progPath);
            break;
        }
        token = strtok(NULL, ":");
    }

    free(pathCopy);
    return result;
}

void waitForChild() {
    int status = 0;
    wait(&status);
    handleChildStatus(status);
}

void handleCommand(char* path, char* args) {
    // printf("dbg: running command: %s with args %s\n", path, args);

    char* pathCopy = strdup(path);
    if (pathCopy == NULL) {
        err(1, "strdup failed\n");
    }

    char* name = basename(pathCopy);
    if (strcmp(path, name) == 0) {
        free(path);
        path = findProgramInPATH(name);
    }
    if (path == NULL) {
        setErrorWithAlloc(UNKNOWN_COMMAND_ERROR, "command not found...", 0);
        return;
    }

    char** argArr = createArgArray(name, args);
    pid_t pid = fork();
    switch (pid) {
    case -1:
        err(1, "fork failed\n");
        break;
    case 0:   // child
        execv(path, argArr);
        break;
    default:   // parent
        waitForChild();
        break;
    }

    free(path);
    free(pathCopy);
    free(args);
    free(argArr);
}

/*
 * frees command name and args
 */
void runCommand(command_t command) {
    char* path = command.name;
    char* args = command.args;

    int customCommandID = getCustomCommandID(path);
    if (customCommandID != -1) {
        char** argArr = createArgArray(path, args);
        handleCustomCommand(customCommandID, argArr);
        free(path);
        free(args);
    } else {
        handleCommand(path, args);
    }
}

/*
 * replaces STDIN/STDOUT with file
 * returns copy of original FD or -1 on error
 */
int replaceFDByFile(int originalFD, char* filename, int oflag, mode_t mode) {
    int originalFDCopy = dup(originalFD);
    close(originalFD);

    int fd = open(filename, oflag, mode);
    if (fd == -1) {
        setErrorWithAlloc(GENERAL_ERROR, strerror(errno), 0);
        return -1;
    } else if (fd != originalFD) {
        setErrorWithAlloc(GENERAL_ERROR,
                          "Failed to replace standard FD by file", 0);
        return -1;
    }

    return originalFDCopy;
}

void replaceFileByFD(int originalFD, int originalFDCopy) {
    // close(originalFD);   // maybe not needed------------------------------
    if (dup2(originalFDCopy, originalFD) == -1) {
        setErrorWithAlloc(GENERAL_ERROR, strerror(errno), 0);
    }
    close(originalFDCopy);
}

/*
 * frees all nested strings in node
 */
void runCommandWithRedirects(command_node_t* node) {
    redirect_t redirect = node->data.redirect;
    int stdinCopy = 0;
    int stdoutCopy = 0;

    if (redirect.inFile != NULL) {
        int oflag = O_RDONLY;
        stdinCopy = replaceFDByFile(STDIN_FILENO, redirect.inFile, oflag, 0);
    }
    if (redirect.outFile != NULL) {
        int appendFlag = redirect.append ? O_APPEND : O_TRUNC;
        int oflag = O_WRONLY | appendFlag | O_CREAT;
        stdoutCopy =
            replaceFDByFile(STDOUT_FILENO, redirect.outFile, oflag, 0644);
    }

    if (stdinCopy == -1 || stdoutCopy == -1) {
        return;
    }
    runCommand(node->data.command);

    if (redirect.inFile != NULL) {
        replaceFileByFD(STDIN_FILENO, stdinCopy);
        free(redirect.inFile);
    }
    if (redirect.outFile != NULL) {
        replaceFileByFD(STDOUT_FILENO, stdoutCopy);
        free(redirect.outFile);
    }
}

command_head_t* createCommandHead() {
    command_head_t* result = malloc(sizeof(command_head_t));
    STAILQ_INIT(result);
    return result;
}

command_node_t* createCommandNode(command_with_redirects_t data) {
    command_node_t* result = malloc(sizeof(command_node_t));
    result->data = data;
    return result;
}

void addCommandNode(command_head_t* head, command_node_t* node) {
    STAILQ_INSERT_TAIL(head, node, entries);
}

void freeCommandQueue(command_head_t* head) {
    command_node_t* iter = STAILQ_FIRST(head);
    while (iter != NULL) {
        command_node_t* next = STAILQ_NEXT(iter, entries);
        free(iter);
        iter = next;
    }

    free(head);
}

void runCommandsInQueue(command_head_t* head) {
    command_node_t* iter;
    STAILQ_FOREACH(iter, head, entries) {
        // printf("running command: %s\n", iter->data.command.name); //dbg
        runCommandWithRedirects(iter);
    }
    // MAYBE TODO: free nested struct while freeing queue
    freeCommandQueue(head);
}