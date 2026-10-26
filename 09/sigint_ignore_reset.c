
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(void)
{
    if (signal(SIGINT, SIG_IGN) == SIG_ERR)
    {
        perror("signal");
        return EXIT_FAILURE;
    }

    printf("SIGINT is now ignored.\n");
    printf("Press Ctrl+C during the next 5 seconds.\n");
    fflush(stdout);

    sleep(5);

    if (signal(SIGINT, SIG_DFL) == SIG_ERR)
    {
        perror("signal");
        return EXIT_FAILURE;
    }

    printf("\nDefault SIGINT action restored.\n");
    printf("Now press Ctrl+C to terminate the program.\n");
    fflush(stdout);

    while (1)
    {
        pause();
    }

    return 0;
}
