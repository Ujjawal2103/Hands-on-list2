
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

void handle_vtalarm(int sig)
{
    const char msg[] = "ITIMER_VIRTUAL expired: SIGVTALRM received\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

int main(void)
{
    struct itimerval timer;

    if (signal(SIGVTALRM, handle_vtalarm) == SIG_ERR) {
        perror("signal");
        return 1;
    }

    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 10;

    timer.it_interval.tv_sec = 10;
    timer.it_interval.tv_usec = 10;

    if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1) {
        perror("setitimer");
        return 1;
    }

    printf("ITIMER_VIRTUAL timer started.\n");
    printf("It counts user-mode CPU time, not elapsed wall-clock time.\n");
    printf("The timer expires after 10 seconds and 10 microseconds of user CPU time,\n");
    printf("then repeats at the same interval. Press Ctrl+C to stop.\n");
    fflush(stdout);

    while (1) {
    /* Consume user-mode CPU time so ITIMER_VIRTUAL can expire. */
}
return 0;
}
