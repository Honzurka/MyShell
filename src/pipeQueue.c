#include "pipeQueue.h"
#include "commandQueue.h"
#include "globalError.h"
#include "helpers.h"
#include <stdlib.h>
#include <stdio.h>   //dbg
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

void closePipesExcept(int pipes[], int pipesSize, int fd1, int fd2) {
    for (int i = 0; i < pipesSize; i++) {
        if (pipes[i] == fd1 || pipes[i] == fd2) {
            continue;
        }
        close(pipes[i]);
    }
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

void runCommandsByChild(int childIdx, command_head_t* command_head, int pipes[],
                        int pipeCount) {
    int readFD = childIdx == 0 ? STDIN_FILENO : pipes[2 * (childIdx - 1)];
    int writeFD =
        childIdx == pipeCount ? STDOUT_FILENO : pipes[2 * childIdx + 1];
    closePipesExcept(pipes, 2 * pipeCount, readFD, writeFD);
    runCommandsInQueue(command_head, readFD, writeFD);
}

/*
 * Runs all piped commands from queue except last command which is run by parent
 */
void runPipesInQueue(pipe_head_t* head) {
    int pipeCount = getQueueSize(head) - 1;
    int childCount = pipeCount;

    // create pipes
    int pipes[2 * pipeCount];
    for (int i = 0; i < pipeCount; i++) {
        if (pipe(&pipes[2 * i]) == -1) {
            err(1, "%s", strerror(errno));
        }
    }

    // create child processes
    int childIdx = 0;
    pipe_node_t* iter;
    STAILQ_FOREACH(iter, head, entries) {
        if (childIdx == childCount) {
            break;
        }

        pid_t pid = fork();
        switch (pid) {
        case -1:   // error
            err(1, "%s", strerror(errno));
        case 0:
            runCommandsByChild(childIdx, iter->data, pipes, pipeCount);
            _exit(0);
        default:
            break;
        }
        childIdx++;
    }

    runCommandsByChild(childIdx, iter->data, pipes, pipeCount);

    while (childCount > 0) {
        waitForChild();
        childCount--;
    }

    freePipeQueue(head);
}