
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum)
{
    const char message[] = "\nSIGINT received! Ctrl+C was pressed.\n";

    write(STDOUT_FILENO, message, sizeof(message) - 1);
}

int main(void)
{
    if (signal(SIGINT, handler) == SIG_ERR)
    {
        perror("signal");
        return EXIT_FAILURE;
    }

    printf("SIGINT handler registered.\n");
    printf("Press Ctrl+C to test the handler.\n");

    while (1)
    {
        pause();
    }

    return 0;
}
