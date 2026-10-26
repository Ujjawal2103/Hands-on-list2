
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

void handler(int signum)
{
    const char message[] = "SIGVTALRM received! Virtual timer expired.\n";

    write(STDOUT_FILENO, message, sizeof(message) - 1);
    _exit(EXIT_SUCCESS);
}

int main(void)
{
    struct itimerval timer;

    if (signal(SIGVTALRM, handler) == SIG_ERR)
    {
        perror("signal");
        return EXIT_FAILURE;
    }

    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    printf("Setting ITIMER_VIRTUAL for 1 second of CPU time...\n");
    fflush(stdout);

    if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1)
    {
        perror("setitimer");
        return EXIT_FAILURE;
    }

    /* Consume user-mode CPU time until SIGVTALRM arrives. */
    volatile unsigned long counter = 0;

    while (1)
    {
        counter++;
    }

    return 0;
}
