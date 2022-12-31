#include "parserFunctions.h"
#include "helpers.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

/*
 * Concatenates two strings, freeing the originals
 */
char* concatArgs(char* str1, char* str2) {
    int len = 0;
    len += strlen(str1);
    len += strlen(str2);

    char* result = malloc(len + 2);
    if (result == NULL) {
        err(1, "malloc failed\n");
    }

    sprintf(result, "%s %s", str1, str2);

    free(str1);
    free(str2);

    return result;
}

redirect_t createRedirect(char* file, redirectType type) {
    redirect_t result = {0};

    switch (type) {
    case INPUT:
        result.inFile = file;
        break;
    case OUTPUT:
        result.outFile = file;
        result.append = 0;
        break;
    case APPEND:
        result.outFile = file;
        result.append = 1;
        break;
    default:
        err(1, "invalid redirect type\n");
    }

    return result;
}

redirect_t combineRedirects(redirect_t this, redirect_t other) {
    if (other.inFile != NULL) {
        free(this.inFile);
        this.inFile = other.inFile;
    }
    if (other.outFile != NULL) {
        free(this.outFile);
        this.outFile = other.outFile;
        this.append = other.append;
    }

    return this;
}

command_t createCommand(char* name, char* args) {
    command_t result = {0};

    result.name = name;
    result.args = args;

    return result;
}

command_with_redirects_t createCommandWithRedirects(command_t command,
                                                    redirect_t redirect) {
    command_with_redirects_t result = {0};

    result.command = command;
    result.redirect = redirect;

    return result;
}