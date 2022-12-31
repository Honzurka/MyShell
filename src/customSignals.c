#include "customSignals.h"
#include <err.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>   //dbg
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

void sigintHandler(int signum) {
    (void)signum;
    // MAYBE TODO: ignore other signals
    write(STDOUT_FILENO, "\n", 1);   // signal-safe alternative to printf

    RL_UNSETSTATE(RL_STATE_ISEARCH | RL_STATE_NSEARCH | RL_STATE_VIMOTION |
                  RL_STATE_NUMERICARG | RL_STATE_MULTIKEY);
    rl_reset_line_state();
    rl_replace_line("", 0);
    rl_redisplay();
}

/*
 * ignores SIGCHLD without SIG_IGN => allows reading exit value after wait()
 */
void sigchldHandler(int signum) { (void)signum; }

void configureSignalHandling() {
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sa.sa_handler = sigintHandler;
    if (sigaction(SIGINT, &sa, NULL) != 0) {
        err(1, "Can't catch SIGINT\n");
    }

    sa.sa_handler = sigchldHandler;
    if (sigaction(SIGCHLD, &sa, NULL) != 0) {
        err(1, "Can't catch SIGCHLD\n");
    }
}