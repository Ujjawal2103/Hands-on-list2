
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

void handle_alarm(int sig)
{
    const char msg[] = "ITIMER_REAL expired: SIGALRM received\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

int main(void)
{
    struct itimerval timer;

    if (signal(SIGALRM, handle_alarm) == SIG_ERR) {
        perror("signal");
        return 1;
    }

    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 10;

    timer.it_interval.tv_sec = 10;
    timer.it_interval.tv_usec = 10;

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("setitimer");
        return 1;
    }

    printf("ITIMER_REAL timer started.\n");
    printf("It expires after 10 seconds and 10 microseconds,\n");
    printf("then repeats at the same interval. Press Ctrl+C to stop.\n");
    fflush(stdout);

    while (1) {
        pause();
    }

    return 0;
}
