#include "customSignals.h"
#include <err.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>   //dbg
#include <readline/readline.h>
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

void configureSignalHandling() {
    struct sigaction sigint_sa;
    sigemptyset(&sigint_sa.sa_mask);
    sigint_sa.sa_flags = 0;

    sigint_sa.sa_handler = sigintHandler;
    if (sigaction(SIGINT, &sigint_sa, NULL) != 0) {
        err(1, "Can't catch SIGINT\n");
    }

    sigint_sa.sa_handler = SIG_IGN;
    if (sigaction(SIGCHLD, &sigint_sa, NULL) != 0) {
        err(1, "Can't catch SIGCHLD\n");
    }
}