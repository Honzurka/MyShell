#include "pipeQueue.h"
#include "commandQueue.h"
#include <stdlib.h>
#include <stdio.h>   //dbg

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

void runPipesInQueue(pipe_head_t* head) {
    pipe_node_t* iter;
    STAILQ_FOREACH(iter, head, entries) {
        printf("running pipe\n");
        runCommandsInQueue(iter->data);
    }
}