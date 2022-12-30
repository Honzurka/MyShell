#include "commandQueue.h"
#include <stdlib.h>

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
