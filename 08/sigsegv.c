
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum)
{
    const char message[] = "SIGSEGV signal received\n";
    write(STDOUT_FILENO, message, sizeof(message) - 1);
    _exit(EXIT_FAILURE);
}

int main(void)
{
    if (signal(SIGSEGV, handler) == SIG_ERR)
    {
        perror("signal");
        return EXIT_FAILURE;
    }

    printf("SIGSEGV handler registered.\n");
    printf("Program is running. Press Ctrl+C to stop it.\n");

    while (1)
    {
        pause();
    }

    return 0;
}
