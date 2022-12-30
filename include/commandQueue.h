#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H

#include "parserFunctions.h"
#include <sys/queue.h>

typedef struct command_node {
    command_with_redirects_t data;
    STAILQ_ENTRY(command_node) entries;
} command_node_t;

typedef STAILQ_HEAD(command_head, command_node) command_head_t;

command_head_t* createCommandHead();
command_node_t* createCommandNode(command_with_redirects_t data);
void addCommandNode(command_head_t* head, command_node_t* node);
void runCommandsInQueue(command_head_t* head, int readFD, int writeFD);

#endif