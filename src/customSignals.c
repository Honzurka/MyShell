#include "customSignals.h"
#include <err.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h> //dbg
#include <readline/readline.h>
#include <unistd.h>

void sigint_handler(int signum)
{
    write(STDOUT_FILENO, "\n", 1); //signal-safe alternative to printf
    
    RL_UNSETSTATE(RL_STATE_ISEARCH|RL_STATE_NSEARCH|RL_STATE_VIMOTION|RL_STATE_NUMERICARG|RL_STATE_MULTIKEY);
    rl_reset_line_state();
    rl_replace_line("", 0);
    rl_redisplay();
}

void configureSignalHandling()
{
    struct sigaction sigint_sa;
    sigint_sa.sa_handler = sigint_handler;
    sigemptyset(&sigint_sa.sa_mask);
    sigint_sa.sa_flags = 0;
    if (sigaction(SIGINT, &sigint_sa, NULL) != 0)
    {
        err(1, "Can't catch SIGINT\n");
    }

    //... other signals
}