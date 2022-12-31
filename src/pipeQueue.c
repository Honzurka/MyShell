#include "pipeQueue.h"
#include "commandQueue.h"
#include "globalError.h"
#include "helpers.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

pipe_head_t* createPipeHead() {
    pipe_head_t* result = malloc(sizeof(pipe_head_t));
    STAILQ_INIT(result);
    return result;
}

pipe_node_t* createPipeNode(command_head_t* data) {
    pipe_node_t* result = malloc(sizeof(pipe_node_t));
    result->data = data;
    return result;
}

void addPipeNode(pipe_head_t* head, pipe_node_t* node) {
    STAILQ_INSERT_TAIL(head, node, entries);
}

int getQueueSize(pipe_head_t* head) {
    int result = 0;
    pipe_node_t* iter;
    STAILQ_FOREACH(iter, head, entries) { result++; }
    return result;
}

void freePipeQueue(pipe_head_t* head) {
    pipe_node_t* iter = STAILQ_FIRST(head);
    while (iter != NULL) {
        pipe_node_t* next = STAILQ_NEXT(iter, entries);
        free(iter);
        iter = next;
    }
    free(head);
}

void runCommandsByChild(command_head_t* command_head, int readFD, int writeFD) {
    runCommandsInQueue(command_head, readFD, writeFD);
}

/*
 * Runs all piped commands from queue except last command which is run by parent
 */
void runPipesInQueue(pipe_head_t* head) {
    int childCount = getQueueSize(head) - 1;
    int oldPipeFD[2] = {STDIN_FILENO, STDOUT_FILENO};
    int pipeFD[2] = {-1, -1};

    // create child processes
    int childIdx = 0;
    pipe_node_t* iter;
    STAILQ_FOREACH(iter, head, entries) {
        if (childIdx == childCount) {
            break;
        }

        if (pipe(pipeFD) == -1) {
            err(1, "pipe creation failed: %s\n", strerror(errno));
        }

        pid_t pid = fork();
        switch (pid) {
        case -1:   // error
            err(1, "fork failed: %s\n", strerror(errno));
        case 0:
            safeCloseUnlessStandard(pipeFD[0], "closing pipe failed");
            safeCloseUnlessStandard(oldPipeFD[1], "closing pipe failed");
            runCommandsByChild(iter->data, oldPipeFD[0], pipeFD[1]);
            _exit(0);
        default:
            break;
        }

        safeCloseUnlessStandard(oldPipeFD[0], "closing pipe failed");
        safeCloseUnlessStandard(oldPipeFD[1], "closing pipe failed");
        oldPipeFD[0] = pipeFD[0];
        oldPipeFD[1] = pipeFD[1];
        childIdx++;
    }

    safeCloseUnlessStandard(oldPipeFD[1], "closing pipe failed");
    runCommandsByChild(iter->data, oldPipeFD[0], STDOUT_FILENO);

    while (childCount > 0) {
        waitForChild(-1);
        childCount--;
    }

    freePipeQueue(head);
}