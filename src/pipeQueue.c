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

void closeFDsExcept(int fds[], int fdsSize, int fd1, int fd2) {
    for (int i = 0; i < fdsSize; i++) {
        if (fds[i] == fd1 || fds[i] == fd2) {
            continue;
        }
        close(fds[i]);
    }
}

void runPipesInQueue(pipe_head_t* head) {
    int childCount = getQueueSize(head);
    int pipeCount = childCount - 1;

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
        command_head_t* command_head = iter->data;
        int readFD, writeFD;
        pid_t pid = fork();
        switch (pid) {
        case -1:   // error
            err(1, "%s", strerror(errno));
        case 0:   // child
            readFD = childIdx == 0 ? STDIN_FILENO : pipes[2 * (childIdx - 1)];
            writeFD = childIdx == childCount - 1 ? STDOUT_FILENO
                                                 : pipes[2 * childIdx + 1];

            closeFDsExcept(pipes, 2 * pipeCount, readFD, writeFD);

            // replace stdin/stdout with pipes
            if (readFD != STDIN_FILENO) {
                if (dup2(readFD, STDIN_FILENO) == -1) {
                    err(1, "%s", strerror(errno));
                }
                close(readFD);
            }
            if (writeFD != STDOUT_FILENO) {
                if (dup2(writeFD, STDOUT_FILENO) == -1) {
                    err(1, "%s", strerror(errno));
                }
                close(writeFD);
            }

            runCommandsInQueue(command_head);

            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            return;   // child execution ends here
        default:      // parent
            break;
        }
        childIdx++;
    }

    // parent will close pipes
    for (int i = 0; i < pipeCount; i++) {
        close(pipes[2 * i]);
        close(pipes[2 * i + 1]);
    }

    // parent will wait for all children
    while (childCount > 0) {
        waitForChild();
        childCount--;
    }

    // TODO: free queue
}