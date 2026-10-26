
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

void handler(int signum)
{
    const char message[] = "SIGALRM received! Timer expired.\n";

    write(STDOUT_FILENO, message, sizeof(message) - 1);
}

int main(void)
{
    struct itimerval timer;

    if (signal(SIGALRM, handler) == SIG_ERR)
    {
        perror("signal");
        return EXIT_FAILURE;
    }

    timer.it_value.tv_sec = 5;
    timer.it_value.tv_usec = 0;

    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    printf("Setting ITIMER_REAL for 5 seconds...\n");
    fflush(stdout);

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
    {
        perror("setitimer");
        return EXIT_FAILURE;
    }

    pause();

    printf("Program continues after handling SIGALRM.\n");

    return 0;
}
