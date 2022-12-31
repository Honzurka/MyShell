#include "helpers.h"
#include "globalError.h"
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

char* allocateString(char* str) {
    char* result = malloc(strlen(str) + 1);
    if (result == NULL) {
        err(1, "malloc failed\n");
    }
    strcpy(result, str);
    return result;
}

/*
 * Returns the number of arguments in NULL-terminated array of strings.
 */
int getArgCount(char** str) {
    int result = 0;
    while (str[result] != NULL) {
        result++;
    }
    return result;
}

int countCharOccurencesInStr(char c, char* str) {
    int result = 0;
    for (char* str_p = str; *str_p != '\0'; str_p++) {
        if (*str_p == c) {
            result++;
        }
    }
    return result;
}

/*
 * childPid = 1 -> wait for any child
 */
void waitForChild(int childPid) {
    int status = 0;
    int pid = waitpid(childPid, &status, 0);
    if (pid == -1) {
        if (errno == EINTR) {
            setErrorWithAlloc(SIGNAL_BASE + SIGINT, "wait interrupted", 0);
        } else {
            err(1, "wait failed: %s\n", strerror(errno));
        }
    } else {
        handleChildStatus(status);
    }
}

/*
 * duplicates fd and returns it
 * on error, prints error message and exits
 */
int safeDup(int fd) {
    int result = dup(fd);
    if (result == -1) {
        err(1, "safeDup failed: %s\n", strerror(errno));
    }
    return result;
}

/*
 * puts oldfd into newfd
 * on error, prints error message and exits
 */
void safeDup2(int oldfd, int newfd) {
    if (dup2(oldfd, newfd) == -1) {
        err(1, "safeDup2 failed: %s\n", strerror(errno));
    }
    safeClose(oldfd, "safeDup2 close failed");
}

void safeClose(int fd, char* errorMessage) {
    if (close(fd) == -1) {
        err(1, "%s: %s\n", errorMessage, strerror(errno));
    }
}

void safeCloseUnlessStandard(int fd, char* errorMessage) {
    if (fd != STDIN_FILENO && fd != STDOUT_FILENO) {
        safeClose(fd, errorMessage);
    }
}