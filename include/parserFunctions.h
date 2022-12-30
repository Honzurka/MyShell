#ifndef PARSER_FUNCTIONS_H
#define PARSER_FUNCTIONS_H

#include <sys/queue.h>

typedef enum { INPUT, OUTPUT, APPEND } redirectType;

typedef struct {
    char* inFile;
    char* outFile;
    int append;
} redirect_t;

typedef struct {
    char* name;
    char* args;
} command_t;

typedef struct {
    command_t command;
    redirect_t redirect;
} command_with_redirects_t;

// typedef struct command_node {
//     command_with_redirects_t data;
//     STAILQ_ENTRY(command_node) entries;
// } command_node_t;

char* concatArgs(char* str1, char* str2);
void runCommand(char* name, char* args);
redirect_t createRedirect(char* file, redirectType type);
redirect_t combineRedirects(redirect_t this, redirect_t other);
command_t createCommand(char* name, char* args);
command_with_redirects_t createCommandWithRedirects(command_t command,
                                                    redirect_t redirect);

#endif