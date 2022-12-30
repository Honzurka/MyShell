#ifndef PIPE_QUEUE_H
#define PIPE_QUEUE_H

#include "commandQueue.h"
#include <sys/queue.h>

typedef struct pipe_node {
    command_head_t* data;
    STAILQ_ENTRY(pipe_node) entries;
} pipe_node_t;

typedef STAILQ_HEAD(pipe_head, pipe_node) pipe_head_t;

pipe_head_t* createPipeHead();
pipe_node_t* createPipeNode(command_head_t* data);
void addPipeNode(pipe_head_t* head, pipe_node_t* node);
void runPipesInQueue(pipe_head_t* head);

#endif