
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum)
{
    const char message[] = "SIGALRM received! Timer expired.\n";

    write(STDOUT_FILENO, message, sizeof(message) - 1);
}

int main(void)
{
    if (signal(SIGALRM, handler) == SIG_ERR)
    {
        perror("signal");
        return EXIT_FAILURE;
    }

    printf("Alarm set for 5 seconds...\n");
    fflush(stdout);

    alarm(5);

    pause();

    printf("Program continues after handling SIGALRM.\n");

    return 0;
}
