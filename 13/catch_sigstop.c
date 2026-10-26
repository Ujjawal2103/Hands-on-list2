
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum)
{
    printf("SIGSTOP caught!\n");
}

int main(void)
{
    if (signal(SIGSTOP, handler) == SIG_ERR) {
        perror("Cannot register SIGSTOP handler");
    } else {
        printf("SIGSTOP handler registered.\n");
    }

    printf("Process PID: %d\n", getpid());
    printf("Waiting for signals...\n");
    fflush(stdout);

    while (1) {
        pause();
    }

    return 0;
}
